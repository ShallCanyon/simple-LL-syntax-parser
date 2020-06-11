#include "LL.h"

LL::LL()
{
}
LL::~LL()
{
}

void LL::loadFile(std::fstream &file)
{
	std::string line;
	//int count = 0;
	while (!file.eof())
	{
		getline(file, line);
		preProcess(line/*, count*/);
	}
}

void LL::createTable()
{
	generateFirst();
	generateFollow();
	initRawColHeader();

}



void LL::generateFirst()
{
	std::string N, tmpStr;
	std::set<char> set;
	charSet::iterator firstIter, tmpIter;
	std::vector<std::string>::iterator strIter;
	int strIterLoc, j, k;
	char tmpChar;
	bool needRescan = false, firstFound, hasEpisilon, allHaveEpisilon;
	//循环执行直到所有文法处理完毕
	do
	{
		needRescan = false;
		//从下到上处理文法
		for (auto rawIter = rawSyntax.rbegin(); rawIter != rawSyntax.rend(); rawIter++)
		{
			//获取非终结符并在first集中寻找
			N = rawIter->first;
			firstIter = firstSet.find(N);
			set.clear();
			//寻找该非终结符的文法内是否有也未写入first集的非终结符
			/* 循环second的每一条字符串 */
			for (strIter = rawIter->second.begin(); strIter != rawIter->second.end(); strIter++)
			{
				strIterLoc = 0;
				allHaveEpisilon = true;
				//某一段文法内可直接写入终结符或空
				tmpChar = strIter->at(0);
				if (tmpChar == EPISILON  || isTerminate(tmpChar))
				{
					if (firstIter != firstSet.end())
					{
						//如果first(N)存在且其中没有该终结符
						if (firstIter->second.find(tmpChar) == firstIter->second.end())
						{
							needRescan = true;
							set.insert(tmpChar);
						}
					}
					else
					{
						needRescan = true;
						set.insert(tmpChar);
					}
				}
				//某一段文法内存在非终结符
				else if(isNonTerminate(tmpChar))
				{ 
					firstFound = true;
					//循环寻找非终结符，并停止在不含ε的非终结符位置
					while (isNonTerminate(strIter->at(strIterLoc)))
					{
						//TODO: this boolean remains bug
						hasEpisilon = false;
						tmpStr.clear();
						//获取非终结符
						if (!checkApostrophe(strIter, strIterLoc, tmpStr))
						{
							printf("Finding nonterminate error: out of range\n");
							exit(-1);
						}
						if (!tmpStr.empty())
						{
							//寻找该非终结符是否有first集
							tmpIter = firstSet.find(tmpStr);
							//该非终结符没有first集，跳到下一条字符串
							if (tmpIter == firstSet.end())
							{
								needRescan = true;
								firstFound = false;
								break;
							}
							//该非终结符有first集
							else
							{
								//将非ε元素写入first集
								for (auto setIter = tmpIter->second.begin(); setIter != tmpIter->second.end(); setIter++)
								{
									if (*setIter != EPISILON)
									{
										if (firstIter != firstSet.end())
										{
											if (firstIter->second.find(*setIter) == firstIter->second.end())
											{
												needRescan = true;
												set.insert(*setIter);
											}
										}
										else
										{
											needRescan = true;
											set.insert(*setIter);
										}
									}
									else
										hasEpisilon = true;
								}
								//若该非终结符first集有ε，继续寻找接下来的非终结符
								//若没有ε，停止寻找
								allHaveEpisilon &= hasEpisilon;
								if (!hasEpisilon)
									break;
							}
						}
						else
						{
							printf("get nonterminate string error\n");
							exit(-1);
						}
						if(strIterLoc >= strIter->size())
							break;
					}
					//存在有非终结符没有first集
					if (!firstFound)
						continue;
					//遇到所有非终结符的first集都有ε, 若其后有终结符，将该终结符写入first(N)
					if (allHaveEpisilon)
					{
						
						//若其后有终结符，将该终结符写入first(N)
						if (strIterLoc < strIter->size() && isTerminate(strIter->at(strIterLoc)))
						{
							if (firstIter != firstSet.end())
							{
								//如果first(N)存在且其中没有该终结符
								if (firstIter->second.find(strIter->at(strIterLoc)) == firstIter->second.end())
								{
									needRescan = true;
									set.insert(strIter->at(strIterLoc));
								}
							}
							else
							{
								needRescan = true;
								set.insert(strIter->at(strIterLoc));
							}
						}
						else
							set.insert(EPISILON);
					}
				}
				//如果first(N)已存在，将新数据写入
				if(firstIter!=firstSet.end())
				{
					firstIter->second.insert(set.begin(), set.end());
				}
				//如果first(N)不存在，写入新数据对
				else
				{
					firstSet.insert(std::pair<std::string, std::set<char>>(N, set));
					firstIter = firstSet.find(N);
				}
			}
		}
	} while (needRescan);

}

void LL::generateFollow()
{
	std::string globalLeftN, localLeftN, localRightN;
	std::set<char> set;
	std::vector<std::string> rightStr;
	std::vector<std::string>::iterator strIter;
	strSet::iterator globalIter, localIter;
	bool needRescan = false, hasEpisilon = false, allHaveEpisilon = true;
	std::size_t loc, length;
	//std::string::size_type loc;

	//initialization
	set.insert('#');
	followSet.insert(std::pair<std::string, std::set<char>>(beginSymbol, set));
	set.clear();

	do
	{
		needRescan = false;
		// 从上到下循环所有文法，每行获得文法的左端非终结符
		for (globalIter = rawSyntax.begin(); globalIter != rawSyntax.end(); globalIter++)
		{
			globalLeftN = globalIter->first;
			auto globalLeftFollowIter = followSet.find(globalLeftN);
			// 再次从上到下循环所有文法，每次获得文法右侧字符串集
			for (localIter = rawSyntax.begin(); localIter != rawSyntax.end(); localIter++)
			{
				// 循环右侧字符串集
				localLeftN = localIter->first;
				for (strIter = localIter->second.begin(); strIter != localIter->second.end(); strIter++)
				{
					loc = strIter->find(globalLeftN);
					if(loc == strIter->npos)
						continue;
					length = strIter->size();
					// 若globalLeftN 为长度大于1的字符串（例如B'），将loc移动到该字符串末尾 
					loc += globalLeftN.size() - 1;

					//右侧为空（找到的字符在最右侧）  follow(globalLeftN) += follow(localLeftN)
					if(loc == length - 1)
					{
						auto localLeftFollowIter = followSet.find(localLeftN);
						// follow(localLeftN)存在， 将follow(localLeftN)写入到follow(globalLeftN)
						if (localLeftFollowIter != followSet.end())
						{
							//循环查找follow(localLeftN)中的字符是否是follow(globalLeftN)中的新数据
							for (auto tmpChar = localLeftFollowIter->second.begin(); tmpChar != localLeftFollowIter->second.end(); tmpChar++)
							{
								if (globalLeftFollowIter->second.find(*tmpChar) == globalLeftFollowIter->second.end())
								{
									set.insert(*tmpChar);
									needRescan = true;
								}
							}
						}
						// follow(localLeftN)不存在, 跳过当前字符串，等待下一次global循环
						else
						{
							needRescan = true;
							continue;
						}

					}
					//右侧为终结符, 将该终结符插入follow(globalLeftN)
					else if(isTerminate(strIter->at(loc + 1)))
					{
						if (globalLeftFollowIter->second.find(strIter->at(loc + 1)) == globalLeftFollowIter->second.end())
						{
							needRescan = true;
							set.insert(strIter->at(loc + 1));
						}
					}
					//右侧为非终结符, 将first(localRightN) - '$' 写入 follow(globalLeftN), 检查first(localRightN)中是否有'$'
					else if (isNonTerminate(strIter->at(loc + 1)))
					{
						allHaveEpisilon = true;
						int i = loc + 1;
						while (i < length && isNonTerminate(strIter->at(i)))
						{
							hasEpisilon = false;
							bool check;
							localRightN.clear();
							check = checkApostrophe(strIter, i, localRightN);
							if (!check)
							{
								printf("Error: Finding nonterminate out of range\n");
								exit(-1);
							}
							auto localRightFirstIter = firstSet.find(localRightN);
							if(localRightFirstIter == firstSet.end())
							{
								printf("Error: FirstSet %s not found\n", localRightN);
								exit(-1);
							}
							for (auto charIter = localRightFirstIter->second.begin(); charIter != localRightFirstIter->second.end(); charIter++)
							{
								if(*charIter==EPISILON)
									hasEpisilon = true;
								else
								{
									if (globalLeftFollowIter->second.find(*charIter) == globalLeftFollowIter->second.end())
									{
										needRescan = true;
										set.insert(*charIter);
									}
								}
							}
							allHaveEpisilon &= hasEpisilon;
							if (!hasEpisilon)
								break;
						}
						if (allHaveEpisilon)
						{
							i--;
							//连续的含$非终止符后有两种情况: 1. 右侧为空
							if (i == length - 1)
							{
								auto localLeftFollowIter = followSet.find(localLeftN);
								// follow(localLeftN)存在， 将follow(localLeftN)写入到follow(globalLeftN)
								if (localLeftFollowIter != followSet.end())
								{
									//循环查找follow(localLeftN)中的字符是否是follow(globalLeftN)中的新数据
									for (auto tmpChar = localLeftFollowIter->second.begin(); tmpChar != localLeftFollowIter->second.end(); tmpChar++)
									{
										if (globalLeftFollowIter->second.find(*tmpChar) == globalLeftFollowIter->second.end())
										{
											set.insert(*tmpChar);
											needRescan = true;
										}
									}
								}
								// follow(localLeftN)不存在, 跳过当前字符串，等待下一次global循环
								else
								{
									needRescan = true;
									continue;
								}
							}
							//2. 右侧为终止符
							else if (isTerminate(strIter->at(i + 1)))
							{
								if (globalLeftFollowIter->second.find(strIter->at(i + 1)) == globalLeftFollowIter->second.end())
								{
									needRescan = true;
									set.insert(strIter->at(i + 1));
								}
							}
						}
					}
				}
			}
			//如果first(N)已存在，将新数据写入
			if (globalLeftFollowIter != followSet.end())
			{
				globalLeftFollowIter->second.insert(set.begin(), set.end());
			}
			//如果first(N)不存在，写入新数据对
			else
			{
				followSet.insert(std::pair<std::string, std::set<char>>(globalLeftN, set));
				//globalLeftFollowIter = followSet.find(globalLeftN);
			}
			set.clear();
		}
	} while (needRescan);
	

}

void LL::preProcess(std::string line/*, int &count*/)
{
	std::string N, tmp;
	std::vector<std::string> syntax;
	strSet::iterator iter;
	int div, syntaxLoc = 0, subSyntaxLoc = 0, loc = 0;
	if (line.length() > 0)
	{
		// 获取"->"的位置
		div = getDiv(line);
		if (div == -1)
		{
			printf("[ERROR]syntax loaded symbol '->' error\n");
			exit(-1);
		}

		for (int i = 0; i < div; i++)
		{
			if (line.at(i) != ' ')
				N.push_back(line.at(i));
		}
		//rawSyntax.at(count).N = N;

		for (int i = div + 2; i < line.length(); i++)
		{
			tmp.clear();
			if (line.at(i) == ' ' || line.at(i) == '|')
				continue;
			while (i < line.length() && line.at(i) != '|' && line.at(i) != ' ')
				tmp += line.at(i++);
			//rawSyntax.at(count).strings.at(syntaxLoc++) = syntax;
			syntax.push_back(tmp);
			loc++;
		}

		iter = rawSyntax.find(N);
		if(beginSymbol.empty())
			beginSymbol = N;
		//非终止符已存在，新的数据写入已有数据之后
		if (iter != rawSyntax.end())
		{
			iter->second.insert(iter->second.end(), syntax.begin(), syntax.end());
		}
		//非终止符不存在，插入新数据对
		else
			rawSyntax.insert(std::pair<std::string, std::vector<std::string>>(N, syntax));
		//count++;
	}
}

void LL::initRawColHeader()
{
	//rmLRecur();
	//rmBacktrack();
	std::string str, tmp;
	int i;	
	terminate.insert("#");
	for (auto line = rawSyntax.begin(); line != rawSyntax.end(); line++)
		if (isNonTerminate(line->first.at(0)))
		{
			nonterminate.insert(line->first);
			for (auto rightStrings = line->second.begin(); rightStrings != line->second.end(); rightStrings++)
				for (auto soloStr = rightStrings->begin(); soloStr != rightStrings->end(); soloStr++)
				{
					str = *soloStr;
					for (i = 0; i < str.size(); i++)
						if (isTerminate(str.at(i)))
						{
							tmp = str.at(i);
							terminate.insert(tmp);
						}
				}
		}
}

void LL::printData()
{
	printf("Begin symbol: %s\n", beginSymbol.c_str());
	printf("Terminate: ");
	for (auto iter = terminate.begin(); iter != terminate.end(); iter++)
		printf("%s ", iter->c_str());
	printf("\n");

	printf("Nonterminate: ");
	for (auto iter = nonterminate.begin(); iter != nonterminate.end(); iter++)
		printf("%s ", iter->c_str());
	printf("\n");

	printf("RawSyntax: \n");
	for (auto iter = rawSyntax.begin(); iter != rawSyntax.end(); iter++)
	{
		printf("%s ->", iter->first.c_str());
		for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
			printf(" %s ", iter2->c_str());
		printf("\n");
	}

	printf("First:\n");
	for (auto iter = firstSet.begin(); iter != firstSet.end(); iter++)
	{
		printf("%s = ", iter->first.c_str());
		for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
			printf(" %c ", *iter2);
		printf("\n");
	}

	printf("Follow:\n");
	for (auto iter = followSet.begin(); iter != followSet.end(); iter++)
	{
		printf("%s = ", iter->first.c_str());
		for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
			printf(" %c ", *iter2);
		printf("\n");
	}
};

bool LL::isTerminate(char data)
{
	if ((data >= 'a' && data <= 'z') || (data >= '(' && data <= '/'))
		return true;
	else
		return false;
	//return !isNonTerminate(data);
}
bool LL::isNonTerminate(char data)
{
	if (data >= 'A' && data <= 'Z')
		return true;
	else
		return false;
}


// void LL::rmLRecur()
// {
// 	std::string N, str;
// 	char factor;
// 	int count, mark;
// 	int *locOfRecur;
// 	for (auto iter = rawSyntax.begin(); iter != rawSyntax.end(); iter++)
// 	{
// 		N = iter->first;
// 		locOfRecur = new int[iter->second.size()];
// 		count = 0, mark = 0;
// 		for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
// 		{
// 			if (iter2->substr(0, N.size() - 1).compare(N) == 0)
// 			{
// 				locOfRecur[mark++] = count;
// 			}
// 			count++;
// 		}
// 		/* left recursive found */
// 		if (mark != 0)
// 		{
// 			//rmLRecur(*iter, locOfRecur, mark);
// 		}
// 	}
// 	delete[] locOfRecur;
// }

// void LL::rmBacktrack()
// {
// 	std::string N, str;
// 	char factor, *firstChar;
// 	int count;
// 	for (auto iter = rawSyntax.begin(); iter != rawSyntax.end(); iter++)
// 	{
// 		N = iter->first;
// 		firstChar = new char[iter->second.size()];
// 		count = 0;
// 		for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
// 		{
// 			firstChar[count] = iter2->at(0);
// 			count++;
// 		}
// 	}
// 	delete[] firstChar;
// }

int LL::getDiv(std::string str)
{
	int loc = -1;
	if (str.length() >= 3) /* Nonterminate word before "->", needs 3 words */
	{
		for (int i = 0; i < str.length() - 1; i++)
			if (str.at(i) == '-' && str.at(i + 1) == '>')
			{
				loc = i;
				break;
			}
	}
	return loc;
}

/* 查询iter->at(loc)位置的非终结符是否为B'类型，即后跟单引号 */
bool LL::checkApostrophe(std::vector<std::string>::iterator iter, int &loc, std::string &str)
{
	if(loc < 0 || loc >= iter->size())
		return false;
	std::string tmp;
	do
	{
		tmp += iter->at(loc);
		loc++;
	} while (loc < iter->size() && iter->at(loc) == '\'');
	str = tmp;
	//nonterminate.insert(str);
	return true;
}
#include "LL.h"

LL::LL(){

}
LL::~LL(){

}

void LL::loadFile(std::fstream file)
{
    std::string line;
    int count = 0;
	while (!file.eof())
    {
		getline(file, line);
		preProcess(line, count);
	}
	// TODO: exercute rmLRecur() and rmBacktrack()
	process();
}

void LL::createTable()
{
	generateFirst();
	generateFollow();
}

void LL::test()
{
	std::vector<char> ch;
	std::string str;
	ch.push_back('H');
	ch.push_back('E');
	ch.push_back('L');
	ch.push_back('L');
	ch.push_back('O');
	set.insert(std::pair<std::string, std::vector<char>>("first", ch));
	set.insert(std::pair<std::string, std::vector<char>>("second", ch));
	set.insert(std::pair<std::string, std::vector<char>>("first", ch));
	std::map<std::string, std::vector<char>>::iterator iter;
	std::vector<char>::iterator iter2;
	for (iter = set.begin(); iter != set.end(); iter++)
	{
		printf("key: %s, value: {", iter->first.c_str());
		for (iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
			printf("%c ", *iter2);
		printf("}\n");
	}
	iter = set.find("first");
	if(iter!=set.end())
	{
		printf("second found, the value is: { ");
		for (iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
			printf("%c ", *iter2);
		printf("}\n");
	}
	else
		printf("Not found\n");
}

void LL::generateFirst()
{
	std::string N;
	std::vector<char> set;
	int i = 0, j, k;
	// for (auto iter = rawSyntax.begin(); iter != rawSyntax.end(); iter++)
	// {
	// 	N = iter->N;
	// 	for (auto iter2 = iter->strings.begin(); iter2 != iter->strings.end(); iter2++)
	// 	{
	// 		if (isTerminate(iter2->at(0)) /*|| iter2->at(0) == ε*/)
	// 		{
	// 			set.push_back(iter2->at(0));
	// 		}
	// 	}
	// 	firstSet.at(i).N = N;
	// 	firstSet.at(i).ch = set;
	// 	i++;
	// 	set.clear();
	// }
	// i = 0;
	// set.clear();
	// // S -> AB
	// for (auto iter = rawSyntax.begin(); iter != rawSyntax.end(); iter++)
	// {
	// 	// S
	// 	N = iter->N;
	// 	// AB
	// 	for (auto strings = iter->strings.begin(); strings != iter->strings.end(); strings++)
	// 	{
	// 		// AB
	// 		for (j = 0; j < strings->size(); j++)
	// 		{
	// 			if (isNonTerminate(strings->at(j)))
	// 			{
	// 				// First(A) = {Ca | ε}
	// 				// First(B) = {cB`}
	// 				for (auto firstIter = firstSet.begin(); firstIter != firstSet.end(); firstIter++)
	// 				{
	// 					//TODO: find '`'
	// 					if (firstIter->N.at(0) == strings->at(0))
	// 					{
	// 						// {b}
	// 						for (k = 0; k < firstIter->ch.size(); k++)
	// 						{
	// 							/*if (firstIter->ch.at(k) != 'ε')
	// 								set.push_back(firstIter->ch.at(k));*/
	// 						}
	// 						break;
	// 					}
	// 				}
	// 			}
	// 		}
	// 	}
	// }


}

void LL::generateFollow()
{

}

void LL::preProcess(std::string line, int &count)
{
	std::string N;
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
			if(line.at(i)!= ' ')
				N.push_back(line.at(i));
		}
		//rawSyntax.at(count).N = N;
		
		for (int i = div + 2; i < line.length(); i++)
		{
			if (line.at(i) == ' ' || line.at(i) != '|')
				continue;
			while (line.at(i) != '|' && line.at(i) != ' ')
				syntax.at(loc).push_back(line.at(i++));
			//rawSyntax.at(count).strings.at(syntaxLoc++) = syntax;
			loc++;
		}

		iter = rawSyntax.find(N);
		//非终止符已存在，新的数据写入已有数据之后
		if (iter != rawSyntax.end())
		{
			iter->second.insert(iter->second.end(), syntax.begin(), syntax.end());
		}
		//非终止符不存在，插入新数据对
		else
			rawSyntax.insert(std::pair<std::string, std::vector<std::string>>(N, syntax));
		count++;
	}
}

void LL::process()
{
	//rmLRecur();
	//rmBacktrack();
	for (auto iter = rawSyntax.begin(); iter != rawSyntax.end(); iter++)
	{
		//TODO: fix terminate judgement
		if (isNonTerminate(iter->first.at(0)))
		{
			nonterminate.insert(iter->first);
			for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); iter++)
			{
				if(isTerminate(iter2->at(0)))
					terminate.insert(*iter2);
				else if (isNonTerminate(iter2->at(0)))
					nonterminate.insert(*iter2);
			}
		}
	}
	//std::unique(terminate.begin(), terminate.end());
	//std::unique(nonterminate.begin(), nonterminate.end());
}

bool LL::hasCommonFactor(char *data, char &factor)
{
	return true;
}

void LL::printData()
{
	printf("Terminate: ");
	for (auto iter = terminate.begin(); iter != terminate.end(); iter++)
		printf("%s", *iter);
	printf("\n");

	printf("Nonterminate: ");
	for (auto iter = nonterminate.begin(); iter != nonterminate.end(); iter++)
		printf("%s", *iter);
	printf("\n");

	printf("RawSyntax: \n");
	for (auto iter = rawSyntax.begin(); iter != rawSyntax.end(); iter++)
	{
		printf("%s ->", iter->first);
		for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
			printf(" %s", *iter2);
		printf("\n");
	}

	printf("FinalSyntax: \n");
	for (auto iter = finalSyntax.begin(); iter != finalSyntax.end(); iter++)
	{
		printf("%s ->", iter->first);
		for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
			printf(" %s", *iter2);
		printf("\n");
	}

	printf("First:\n");
	for (auto iter = firstSet.begin(); iter != firstSet.end(); iter++)
	{
		printf("%s ->", iter->first);
		for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
			printf(" %c ", *iter2);
		printf("\n");
	}
	
	printf("Follow:\n");
	for (auto iter = followSet.begin(); iter != followSet.end(); iter++)
	{
		printf("%s ->", iter->first);
		for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
			printf(" %c ", *iter2);
		printf("\n");
	}
	
};

bool LL::isTerminate(char data)
{
	if (data >= 'a' && data <= 'z')
		return true;
	else
		return false;
}
bool LL::isNonTerminate(char data)
{
	if (data >= 'A' && data <= 'Z')
		return true;
	else
		return false;
}

//TODO: not completed
void LL::rmLRecur()
{
	std::string N, str;
	char factor;
	int count, mark;
	int *locOfRecur;
	for (auto iter = rawSyntax.begin(); iter != rawSyntax.end(); iter++)
	{
		N = iter->first;
		locOfRecur = new int[iter->second.size()];
		count = 0, mark = 0;
		for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
		{
			if (iter2->substr(0, N.size() - 1).compare(N) == 0)
			{
				locOfRecur[mark++] = count;
			}
			count++;
		}
		/* left recursive found */
		if (mark != 0)
		{
			//rmLRecur(*iter, locOfRecur, mark);
		}

		
	}
	delete[] locOfRecur;
}
void LL::rmBacktrack()
{
	std::string N, str;
	char factor, *firstChar;
	int count;
	for (auto iter = rawSyntax.begin(); iter != rawSyntax.end(); iter++)
	{
		N = iter->first;
		firstChar = new char[iter->second.size()];
		count = 0;
		for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
		{
			firstChar[count] = iter2->at(0);
			count++;
		}

	}
	delete[] firstChar;
}

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
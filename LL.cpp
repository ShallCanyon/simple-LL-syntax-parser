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

void LL::preProcess(std::string line, int &count)
{
	std::string N, syntax;
	int syntaxLoc = 0, subSyntaxLoc = 0, div;
	if (line.length() > 0)
	{
		div = getDiv(line);
		if (div == -1)
		{
			printf("error syntax loaded\n");
			exit(-1);
		}
		/* insert nonterminate symbol */
		for (int i = 0; i < div; i++)
			N.push_back(line.at(i));
		rawSyntax.at(count).N = N;
		/* symbols should be divided by whitespace */
		for (int i = div + 2; i < line.length(); i++)
		{
			if (line.at(i) == ' ' || line.at(i) != '|')
				continue;
			while (line.at(i) != '|' && line.at(i) != ' ')
				syntax.push_back(line.at(i++));
			rawSyntax.at(count).symbols.at(syntaxLoc++) = syntax;
		}
		count++;
	}
}

void LL::process()
{
	rmLRecur();
	rmBacktrack();
	for (auto iter = rawSyntax.begin(); iter != rawSyntax.end(); iter++)
	{
		if (isNonTerminate(iter->N))
		{
			nonterminate.push_back(iter->N);
			for (auto iter2 = iter->symbols.begin(); iter2 != iter->symbols.end(); iter++)
			{
				if(isTerminate(*iter2))
					terminate.push_back(*iter2);
				else if(isNonTerminate(*iter2))
					nonterminate.push_back(*iter2);
			}
		}
	}
	std::unique(terminate.begin(), terminate.end());
	std::unique(nonterminate.begin(), nonterminate.end());
}

bool LL::hasCommonFactor(char *data, char &factor)
{

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
		printf("%s ->", iter->N);
		for (auto iter2 = iter->symbols.begin(); iter2 != iter->symbols.end(); iter2++)
			printf(" %s", *iter2);
		printf("\n");
	}

	printf("FinalSyntax: \n");
	for (auto iter = finalSyntax.begin(); iter != finalSyntax.end(); iter++)
	{
		printf("%s ->", iter->N);
		for (auto iter2 = iter->symbols.begin(); iter2 != iter->symbols.end(); iter2++)
			printf(" %s", *iter2);
		printf("\n");
	}

	printf("First:\n");
	for (auto iter = firstSet.begin(); iter != firstSet.end(); iter++)
	{
		printf("%s ->", iter->N);
		for (auto iter2 = iter->symbols.begin(); iter2 != iter->symbols.end(); iter2++)
			printf(" %s ", *iter2);
		printf("\n");
	}
	
	printf("Follow:\n");
	for (auto iter = followSet.begin(); iter != followSet.end(); iter++)
	{
		printf("%s ->", iter->N);
		for (auto iter2 = iter->symbols.begin(); iter2 != iter->symbols.end(); iter2++)
			printf(" %s ", *iter2);
		printf("\n");
	}
	
};

bool LL::isTerminate(std::string data)
{
	if (data.at(0) >= 'a' && data.at(0) <= 'z')
		return true;
	else
		return false;
}
bool LL::isNonTerminate(std::string data)
{
	if (data.at(0) >= 'A' && data.at(0) <= 'Z')
		return true;
	else
		return false;
}

void LL::rmLRecur()
{
	std::string N, str;
	char factor;
	int count, mark;
	int *locOfRecur;
	for (auto iter = rawSyntax.begin(); iter != rawSyntax.end(); iter++)
	{
		N = iter->N;
		locOfRecur = new int[iter->symbols.size()];
		count = 0, mark = 0;
		for (auto iter2 = iter->symbols.begin(); iter2 != iter->symbols.end(); iter2++)
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
		N = iter->N;
		firstChar = new char[iter->symbols.size()];
		count = 0;
		for (auto iter2 = iter->symbols.begin(); iter2 != iter->symbols.end(); iter2++)
		{
			firstChar[count] = iter2->at(0);
			count++;
		}

	}
	delete[] firstChar;
}

/* get "->" location in string */
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
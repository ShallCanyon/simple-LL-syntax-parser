#include "LL.h"

LL::LL(){

};
LL::~LL(){

};

void LL::loadFile(std::fstream file)
{
    std::string line;
    int count = 0;
	while (!file.eof())
    {
		getline(file, line);
		preProcess(line, count);
	}
};

void LL::preProcess(std::string line, int count)
{
	std::string N, syntax;
	int syntaxLoc = 0, subSyntaxLoc = 0, div;
	bool sub = false;
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
		finalSyntax.at(count).N = N;
		// TODO: exercute rmLRecur() and rmBacktrack()


		/* symbols should be divided by whitespace */
		for (int i = div; i < line.length(); i++)
		{
			if (line.at(i) == '|')
				sub = true;
			if (line.at(i) == ' ')
				continue;
			while (line.at(i) != ' ')
				syntax.push_back(line.at(i++));
			/* insert symbol before '|' */
			if (!sub)
				finalSyntax.at(count).syntax.at(syntaxLoc++) = syntax;
			/* insert symbol after '|' */
			else
				finalSyntax.at(count).subSyntax.at(subSyntaxLoc++) = syntax;
		}
		count++;
	}
};
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

	printf("FinalSyntax: \n");
	for (auto iter = finalSyntax.begin(); iter != finalSyntax.end(); iter++)
	{
		printf("%s ->", iter->N);
		for (auto iter2 = iter->syntax.begin(); iter2 != iter->syntax.end(); iter2++)
			printf(" %s", *iter2);
		if(!iter->subSyntax.empty())
			for (auto iter2 = iter->subSyntax.begin(); iter2 != iter->subSyntax.end(); iter2++)
				printf("| %s", *iter2);
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

bool LL::isTerminate(char ch)
{

}
bool LL::isNonTerminate(char ch)
{
}
// void LL::syntaxPreProcess()
// {
// }
void LL::rmLRecur(int loc)
{
}
void LL::rmBacktrack(int loc)
{
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
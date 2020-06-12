#include "parseTable.h"

parseTable::parseTable(tableType t, std::string beginSymbol) : table(t), firstSymbol(beginSymbol)
{
    // for (int i = 0; i < nonterminate; i++)
    //     for (int j = 0; j < terminate; j++)
    //     {
    //         td[i][j].N = new char();
    //         td[i][j].T = new char();
    //     }
    symbolTop = -1;
    stringTop = -1;
}

parseTable::~parseTable()
{
    // for (int i = 0; i < nonterminate; i++)
    //     for (int j = 0; j < terminate; j++)
    //     {
    //         delete td[i][j].N;
    //         delete td[i][j].T;
    //         td[i][j].syntax.clear();
    //     }
}

bool parseTable::loadString(char *str)
{
    std::string tmp;
    while(*str!='\0')
    {
        tmp.push_back(*str++);
    }
    if(tmp.size()<=0)
        return false;
    //tmp.reserve();
    return loadString(tmp);
}

bool parseTable::loadString(std::string str)
{
    int top = str.size() - 1;
    if(top < 0)
        return false;
    /* add '#' as end of string */
    stringTop = top + 1;
    stringStack.push_back(toString('#'));
    /* push reversed data into string stack */
    while (top >= 0)
        stringStack.push_back(toString(str.at(top--)));
    return true;
}

// std::vector<char> parseTable::getData(char N, char T)
// {
//     int n, t;
//     n = judgeN(N);
//     t = judgeT(T);
//     if(n == -1 || t == -1)
//     {
//         printf("symbol error\n");
//         exit(-1);
//     }
//     return td[n][t].syntax;
// }

// int parseTable::judgeN(const char *N)
// {
//     int result = -1;
//     if (strcmp(N, "E") == 0)
//         result = 0;
//     else if (strcmp(N, "G") == 0)
//         result = 1;
//     else if (strcmp(N, "T") == 0)
//         result = 2;
//     else if (strcmp(N, "V") == 0)
//         result = 3;
//     else if (strcmp(N, "F") == 0)
//         result = 4;
//     return result;
// }
// int parseTable::judgeN(char N)
// {
//     int result = -1;
//     if (N=='E')
//         result = 0;
//     else if (N=='G')
//         result = 1;
//     else if (N=='T')
//         result = 2;
//     else if (N=='V')
//         result = 3;
//     else if (N=='F')
//         result = 4;
//     return result;
// }

// int parseTable::judgeT(const char *T)
// {
//     int result = -1;
//     if (strcmp(T, "i") == 0)
//         result = 0;
//     else if (strcmp(T, "+") == 0)
//         result = 1;
//     else if (strcmp(T, "*") == 0)
//         result = 2;
//     else if (strcmp(T, "(") == 0)
//         result = 3;
//     else if (strcmp(T, ")") == 0)
//         result = 4;
//     else if (strcmp(T, "#") == 0)
//         result = 5;
//     return result;
// }
// int parseTable::judgeT(char T)
// {
//     int result = -1;
//     if (T == 'i')
//         result = 0;
//     else if (T == '+')
//         result = 1;
//     else if (T == '*')
//         result = 2;
//     else if (T == '(')
//         result = 3;
//     else if (T == ')')
//         result = 4;
//     else if (T == '#')
//         result = 5;
//     return result;
// }

// bool parseTable::fillData(const char *N, const char *T, const char *syntax)
// {
//     int n, t, i = 0;
//     n = judgeN(N);
//     t = judgeT(T);
//     if (n == -1 || t == -1)
//     {
//         printf("input error\n");
//         return false;
//     }
//     strcpy(td[n][t].N, N);
//     strcpy(td[n][t].T, T);
//     while (syntax[i] != '\0')
//         td[n][t].syntax.push_back(syntax[i++]);
//     return true;
// }

// void parseTable::displayTableData()
// {
//     for (int i = 0; i < nonterminate; i++)
//         for (int j = 0; j < terminate; j++)
//         {
//             if (!td[i][j].syntax.empty())
//                 displayTableData(i, j);
//         }
// }

// void parseTable::displayTableData(int x, int y)
// {
//     int length = td[x][y].syntax.size();
//     int i = 0;
//     printf("<%s %s> -> ", td[x][y].N, td[x][y].T);
//     while (i < length)
//         printf("%c ", td[x][y].syntax.at(i++));
//     printf("\n");
// }

// void parseTable::displayTableData(const char *N, const char *T)
// {
//     int n, t;
//     n = judgeN(N);
//     t = judgeT(T);
//     if(n!=-1 || t!=-1)
//         displayTableData(n, t);
// }

std::string parseTable::getData(std::string N, std::string T)
{
    std::string str;
    str = table[N][T];
    auto iter = std::remove_if(str.begin(), str.end(), ::isspace);
    str.erase(iter, str.end());
    return str;
}

bool parseTable::stackExpand()
{
    std::string N = symbolStack.at(symbolTop);
    std::string T = stringStack.at(stringTop);
    std::vector<std::string> container;
    syntaxAtMiddle.clear();
    if (isNonTerminate(N.at(0)))
    {
        /* N and T match syntax, pop out current N and put reversed syntax into stack */
        /*if (isDigit(T))
            T = 'i';*/

        int loc = 0;
        std::string tmp, tableData, leftN;
        do
        {
            leftN += N.at(loc);
            loc++;
        } while (loc < N.size() && N.at(loc) == '\'');

        tableData = getData(leftN, T);
        /* target syntax table has data */
        if(!tableData.empty())
        {
            int i = 0, length;
            /* pop current N */
            symbolStack.pop_back();
            symbolTop--;
            /* push current N */
            syntaxAtMiddle += leftN;
            syntaxAtMiddle += "->";

            if (tableData.at(0) != EPISILON)
            {
                loc = 0;
                while (loc < tableData.size())
                {
                    tmp.clear();
                    do
                    {
                        tmp += tableData.at(loc);
                        loc++;
                    } while (loc < tableData.size() && tableData.at(loc) == '\'');
                    container.push_back(tmp);
                }
                length = container.size() - 1;
                while (length >= 0)
                {
                    symbolStack.push_back(container.at(length--));
                    symbolTop++;
                    syntaxAtMiddle += container.at(i++);
                }
            }
            else
                syntaxAtMiddle += EPISILON;
        }
        /* target syntax table has no data, match error */
        else
        {
            //printf("syntax error\n");
            return false;
        }
    }
    else if(N.size() == 1 && isTerminate(N.at(0)))
    {
        /* N and T match themselves, pop out current top char of pData  */
        if (N == T)
        {
            stringStack.pop_back();
            stringTop--;
            symbolStack.pop_back();
            symbolTop--;
            syntaxAtMiddle += N;
            syntaxAtMiddle += "==";
            syntaxAtMiddle += T;
        }
        else
        {
            //printf("syntax error\n");
            return false;
        }
    }
    else
    {
        printf("Weird error\n");
        return false;
    }
    printStackAndData();
    return true;
}
void parseTable::printStackAndData()
{
    //printf("stack\t current string \t\n");
    int i;
    for (i = 0; i <= symbolTop;i++)
        printf("%s ", symbolStack.at(i).c_str());
    printf("\t\t\t");
    for (i = stringTop; i >=0; i--)
        printf("%s ", stringStack.at(i).c_str());
    printf("\t\t\t");
    for (i = 0; i < syntaxAtMiddle.size(); i++)
        printf("%c", syntaxAtMiddle.at(i));
    printf("\n");
}

void parseTable::process()
{
    bool match;
    if (symbolTop < -1)
    {
        printf("symbolStack top pointer error\n");
        return;
    }
    /* initialize */
    symbolStack.push_back(toString('#'));
    symbolTop++;
    symbolStack.push_back(firstSymbol);
    symbolTop++;

    printf("symbol stack\t\t current string \t\t syntax at middle\n");
    printStackAndData();
    while (symbolTop >= 0 && stringTop >= 0)
    {
        match = stackExpand();
        if (symbolTop < 0 || stringTop < 0 || !match)
        {
            printf("final: syntax error\n");
            exit(-1);
        }
        /* both stacks analyzed completely */
        if (symbolStack.at(symbolTop) == toString('#') && stringStack.at(stringTop) == toString('#'))
        {
            printf("final: syntax analyzed completely\n");
            break;
        }
    }
}

// bool parseTable::isTerminate(char ch)
// {
//     bool match = false;
//     if (ch == 'i')
//         match = true;
//     else if (ch == '+')
//         match = true;
//     else if (ch == '*')
//         match = true;
//     else if (ch == '(')
//         match = true;
//     else if (ch == ')')
//         match = true;
//     else if (ch == '#')
//         match = true;
//     return match;
// }

// bool parseTable::isNonTerminate(char ch)
// {
//     bool match = false;
//     if (ch == 'E')
//         match = true;
//     else if (ch == 'G')
//         match = true;
//     else if (ch == 'T')
//         match = true;
//     else if (ch == 'V')
//         match = true;
//     else if (ch == 'F')
//         match = true;
//     return match;
// }

bool parseTable::isDigit(char ch)
{
    if(ch >= '0' && ch <= '9')
        return true;
    else
        return false;
}
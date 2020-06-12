#include <vector>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "LL.h"
// #define terminate 6
// #define nonterminate 5

// struct tableData
// {
//     char *T;
//     char *N;
//     std::vector<char> syntax;
// };

class parseTable
{
public:
    parseTable(tableType t, std::string beginSymbol);
    ~parseTable();
    /* load inserted string as inside data waiting for being executed */
    bool loadString(char* str);
    /* load inserted string as inside data waiting for being executed */
    bool loadString(std::string str);

    // void displayTableData();
    // void displayTableData(int x, int y);
    // void displayTableData(const char *N, const char *T);
    // /* fill nonterminate N, terminate T and relevant syntax */
    // bool fillData(const char *N, const char *T, const char *syntax);
    void process();

protected:
    /* get syntax from nonterminate N combined with terminate T */
    //std::vector<char> getData(char N, char T);
    std::string getData(std::string N, std::string T);
    bool stackExpand();
    void printStackAndData();
    // /* get number of terminate */
    // int judgeN(const char *N);
    // int judgeN(char N);
    // /* get number of nonterminate */
    // int judgeT(const char *T);
    // int judgeT(char T);

    // bool isTerminate(char ch);
    // bool isNonTerminate(char ch);
    bool isDigit(char ch);

private:
    //tableData td[nonterminate][terminate];
    tableType table;
    /* symbol stack */
    std::vector<std::string> symbolStack;
    int symbolTop;
    /* string data being executed */
    std::vector<std::string> stringStack;
    int stringTop;
    std::string syntaxAtMiddle, firstSymbol;
};
#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <algorithm>

#define EPISILON '$'

typedef std::map<std::string, std::set<char>> charSet;
typedef std::map<std::string, std::vector<std::string>> strSet;
typedef std::map<std::string, std::map<std::string, std::string>> tableType;

class LL
{
public:
    LL();
    ~LL();
    void loadFile(std::fstream &file);
    void createTable();
    void printData();
    tableType getTable();
    std::string getBeginSymbol();
    bool checkApostrophe(std::vector<std::string>::iterator iter, int &loc, std::string &str);

protected:
    
    void preProcess(std::string line /*, int &count*/);
    void initRawColHeader();
    // void rmLRecur();
    // void rmBacktrack();
    int getDiv(std::string str);
    void generateFirst();
    void generateFollow();
    void generateTable();
    
private:
    std::string beginSymbol;
    /* lists of terminate or nonterminate strings */
    std::set<std::string> terminate;
    std::set<std::string> nonterminate;

    strSet rawSyntax;
    charSet firstSet;
    charSet followSet;
    tableType table;
};

bool isTerminate(char data);
bool isNonTerminate(char data);
std::string toString(char data);
std::string toString(char* data);
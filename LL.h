#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <algorithm>

#define EPISILON '$'

typedef std::map<std::string, std::set<char>> charSet;
typedef std::map<std::string, std::vector<std::string>> strSet;

class LL{
    public:
        LL();
        ~LL();
        void loadFile(std::fstream &file);
        void createTable();
        void printData();

    protected:
        bool isTerminate(char data);
        bool isNonTerminate(char data);
        void preProcess(std::string line/*, int &count*/);
        void initRawColHeader();
        // void rmLRecur();
        // void rmBacktrack();
        int getDiv(std::string str);
        void generateFirst();
        void generateFollow();
        void generateTable();
        bool checkApostrophe(std::vector<std::string>::iterator iter, int &loc, std::string &str);

    private:
        std::string beginSymbol;
        /* lists of terminate or nonterminate strings */
        std::set<std::string> terminate;
        std::set<std::string> nonterminate;

        strSet rawSyntax;
        charSet firstSet;
        charSet followSet;
        std::map<std::string, std::map<std::string, std::string>> table;
};

std::string toString(char data);
std::string toString(char* data);
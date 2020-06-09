#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <algorithm>

//#define ε '$'

typedef std::map<std::string, std::set<char>> charSet;
typedef std::map<std::string, std::vector<std::string>> strSet;

class LL{
    public:
        LL();
        ~LL();
        void test();
        void loadFile(std::fstream &file);
        void createTable();
        void printData();

    protected:
        bool isTerminate(char data);
        bool isNonTerminate(char data);
        void preProcess(std::string line, int &count);
        void process();
        //void syntaxPreProcess();
        void rmLRecur();
        void rmBacktrack();
        /* get "->" location in str */
        int getDiv(std::string str);
        void generateFirst();
        void generateFollow();

    private:
        /* lists of terminate or nonterminate strings */
        //std::vector<std::string> terminate;
        std::set<std::string> terminate;
        //std::vector<std::string> nonterminate;
        std::set<std::string> nonterminate;

        /* raw data that loaded from loadFile */
        //std::vector<strSet> rawSyntax;
        strSet rawSyntax;
        /* final data after raw data processed */
        //std::vector<strSet> finalSyntax;
        strSet finalSyntax;
        std::string beginSymbol;
        charSet firstSet;
        charSet followSet;
        
        charSet set;
};

bool checkApostrophe(std::vector<std::string>::iterator iter, int &loc, std::string &str);
bool hasCommonFactor(char *data, char &factor);
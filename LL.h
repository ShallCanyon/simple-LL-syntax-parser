#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <algorithm>

//#define ε '$'

typedef std::map<std::string, std::vector<char>> charSet;
typedef std::map<std::string, std::vector<std::string>> strSet;

/*
typedef struct
{
    std::string N;
    std::vector<std::string> strings;
} strSet;

typedef struct 
{
    std::string N;
    std::vector<char> ch;
}charSet;
*/

class LL{
    public:
        LL();
        ~LL();
        void test();
        void loadFile(std::fstream file);
        void createTable();
        void printData();

    protected:
        bool isTerminate(char data);
        bool isNonTerminate(char data);
        bool hasCommonFactor(char *data, char &factor);
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

        charSet firstSet;
        charSet followSet;
        
        charSet set;
};

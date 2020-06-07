#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

typedef struct
{
    std::string N;
    std::vector<std::string> symbols;
} SET;

class LL{
    public:
        LL();
        ~LL();
        void loadFile(std::fstream file);
        void createTable();
        void printData();

    protected:
        bool isTerminate(std::string data);
        bool isNonTerminate(std::string data);
        bool hasCommonFactor(char *data, char &factor);
        void preProcess(std::string line, int &count);
        void process();
        //void syntaxPreProcess();
        void rmLRecur();
        void rmBacktrack();
        /* get "->" location from one line of rawdata */
        int getDiv(std::string str);
        void generateFirst();
        void generateFollow();

    private:
        /* lists of terminate or nonterminate symbols */        
        std::vector<std::string> terminate;
        std::vector<std::string> nonterminate;
        /* raw data that loaded from loadFile */
        std::vector<SET> rawSyntax;
        /* final data after raw data processed */
        std::vector<SET> finalSyntax;

        std::vector<SET> firstSet;
        std::vector<SET> followSet;
};


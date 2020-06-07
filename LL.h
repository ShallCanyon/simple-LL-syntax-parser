#include <string>
#include <vector>
#include <fstream>

typedef struct
{
    std::string N;
    std::vector<std::string> syntax;
    std::vector<std::string> subSyntax;
} syntaxset;

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
        void printData();

    protected:
        bool isTerminate(char ch);
        bool isNonTerminate(char ch);
        void preProcess(std::string line, int count);
        //void syntaxPreProcess();
        void rmLRecur(int loc);
        void rmBacktrack(int loc);
        int getDiv(std::string str);
    private:
        //std::vector<std::string> rawSyntax;
        std::vector<std::string> terminate;
        std::vector<std::string> nonterminate;
        std::vector<syntaxset> finalSyntax;
        std::vector<SET> firstSet;
        std::vector<SET> followSet;
};


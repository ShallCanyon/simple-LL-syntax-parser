#include "LL.h"

int main(int argc, char const *argv[])
{
    LL l;
    std::fstream input;
    input.open("./testFile.txt", std::ios::in);
    if(input.is_open())
    {
        l.loadFile(input);
        l.createTable();
        l.printData();
    }
    return 0;
}

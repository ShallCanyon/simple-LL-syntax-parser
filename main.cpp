#include "LL.h"

int main(int argc, char const *argv[])
{
    LL l;
    std::fstream input;
    std::string filename;
    //filename = "./test1.txt";
    //filename = "./test2.txt";
    //filename = "./test3.txt";
    //filename = "./test4.txt";
    //filename = "./test5.txt";
    filename = "./test6.txt";
    input.open(filename, std::ios::in);
    if(input.is_open())
    {
        l.loadFile(input);
        l.createTable();
        l.printData();
    }
    return 0;
}

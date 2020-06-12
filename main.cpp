//#include "LL.h"
#include "parseTable.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("syntaxParse: A source file needs to be read\n");
        printf("Try 'syntaxParse -?'\n");
        return 0;
    }
    std::fstream input;
    std::string filename, data;
    LL syntax;
    int o;
    const char *optstring = "s:i:";

    while ((o = getopt(argc, argv, optstring)) != -1)
    {
        switch (o)
        {
        case 's':
            //printf("infile %s opened\n", optarg);
            input.open(optarg, std::ios::in);
            break;
        case 'i':
            //printf("outfile %s opened\n", optarg);
            data = optarg;
            break;
        case '?':
            printf("Usage: syntaxParse [-s][-i]\n");
            printf("  -s source rule file\n");
            printf("  -i input syntax string\n");
            break;
        default:
            printf("Wrong arguments");
            return -1;
            break;
        }
    }
    
    //filename = "./test1.txt";
    //filename = "./test2.txt";
    //filename = "./test3.txt";
    //filename = "./test4.txt";
    //filename = "./test5.txt";
    //filename = "./test6.txt";
    //input.open(filename, std::ios::in);

    if(input.is_open())
    {
        syntax.loadFile(input);
        syntax.createTable();
        syntax.printData();
        tableType table = syntax.getTable();
        parseTable pt(table, syntax.getBeginSymbol());
        //data = "i*(i+i)";
        pt.loadString(data);
        pt.process();
    }

    return 0;
}

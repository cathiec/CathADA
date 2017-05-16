#include "cath_ADA.h"

// argv[1] automaton
// argv[2] -print : print out all the steps

int main(int argc, char ** argv)
{
    bool print = false;

    if(argc > 2)
    {
        std::string temp = argv[2];
        if(temp == "-print")
            print = true;
    }

    cath::ADA a(argv[1], print);

    if(a.is_empty(print))
        std::cout << "EMPTY" << std::endl;
    else
        std::cout << "NOT EMPTY" << std::endl;

    return 0;
}

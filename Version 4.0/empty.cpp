#include "cath_ADA.h"

// argv[1] automaton
// argv[2+]
    // -print : print out all the steps
    // -concrete : use concrete version

int main(int argc, char ** argv)
{
    bool print = false;

    cath::check_mode mode = cath::ABSTRACT;

    for(int i = 2; i < argc; i++)
    {
        std::string temp = argv[i];
        if(temp == "-concrete")
            mode = cath::CONCRETE;
        else if(temp == "-print")
            print = true;
    }

    cath::ADA a(argv[1], print);

    if(a.is_empty(mode, print))
        std::cout << "EMPTY" << std::endl;
    else
        std::cout << "NOT EMPTY" << std::endl;

    return 0;
}

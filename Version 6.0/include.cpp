#include "cath_ADA.h"

// argv[1] automaton 1
// argv[2] automaton 2
// argv[3+]
    // -print : print out all the steps

int main(int argc, char ** argv)
{
    bool print = false;

    for(int i = 3; i < argc; i++)
    {
        std::string temp = argv[i];
        if(temp == "-print")
            print = true;
    }

    cath::ADA a(argv[1], print);
    cath::ADA b(argv[2], print);

    if(a.intersect(b.complement()).is_empty(print))
        std::cout << "L(1) <= L(2)." << std::endl;
    else
        std::cout << "Not included." << std::endl;

    return 0;
}

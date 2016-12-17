#include "cath_ADA.h"

int main(int argc, char ** argv)
{
    cath::ADA a(argv[1]);

    bool print = false;
    cath::check_mode mode = cath::ABSTRACT;

    for(int i = 2; i < argc; i++)
    {
        std::string temp = argv[i];
        if(temp == "-abstract")
            mode = cath::ABSTRACT;
        else if(temp == "-concrete")
            mode = cath::CONCRETE;
        else if(temp == "-print")
            print = true;
    }

    if(a.is_empty(cath::parse("q0"), mode, print))
        std::cout << "EMPTY" << std::endl;
    else
        std::cout << "NOT EMPTY" << std::endl;

    return 0;
}

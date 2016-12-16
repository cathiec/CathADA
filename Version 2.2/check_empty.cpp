#include "cath_ADA.h"

int main(int argc, char ** argv)
{
    cath::ADA a(argv[1]);

    bool print;
    if(std::string(argv[2]) == "true")
        print = true;
    else
        print = false;

    if(a.is_empty(cath::parse("q0"), cath::ABSTRACT, print))
        std::cout << "EMPTY" << std::endl;
    else
        std::cout << "NOT EMPTY" << std::endl;

    return 0;
}

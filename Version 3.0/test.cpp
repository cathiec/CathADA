#include "cath_ADA.h"

int main(int argc, char ** argv)
{
    cath::ADA a("examples/example6.ada");
    cath::ADA b = a.complement();
    std::cout << b << std::endl;
    if(b.is_empty_abstract_mode(true))
        std::cout << "EMPTY" << std::endl;
    else
        std::cout << "NOT EMPTY" << std::endl;

    return 0;
}

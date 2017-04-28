#include "cath_ADA.h"

int main(int argc, char ** argv)
{
    cath::ADA a("examples/example4.ada");
    cath::ADA b = a.complement();
    //std::cout << b << std::endl;
    if(a.is_empty(cath::ABSTRACT ,true))
        std::cout << "EMPTY" << std::endl;
    else
        std::cout << "NOT EMPTY" << std::endl;

    return 0;
}

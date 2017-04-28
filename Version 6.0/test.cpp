#include "cath_ADA.h"

int main(int argc, char ** argv)
{
    cath::ADA a("examples/example4.ada", true);
    if(a.is_empty(true))
        std::cout << "EMPTY" << std::endl;
    else
        std::cout << "NOT EMPTY" << std::endl;
    //std::cout << a.complement() << std::endl;

    return 0;
}

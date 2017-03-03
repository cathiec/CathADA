#include "cath_ADA.h"

int main(int argc, char ** argv)
{
    cath::ADA a("examples/example1.ada", true);
    if(a.is_empty(cath::IMPACT ,true))
        std::cout << "EMPTY" << std::endl;
    else
        std::cout << "NOT EMPTY" << std::endl;

    return 0;
}

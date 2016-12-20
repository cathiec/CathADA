#include "cath_ADA.h"

int main(int argc, char ** argv)
{
    cath::ADA a("examples/example3.ada", true);
    a.is_empty_abstract_mode(true);
    std::cout << std::endl;

    /*cath::ADA c = a.intersect(a.complement());
    std::cout << c << std::endl;

    try{
        if(c.is_empty_abstract_mode(true))
            std::cout << "EMPTY" << std::endl;
        else
            std::cout << "NOT EMPTY" << std::endl;
    }catch(z3::exception e)
    {
        std::cout << e << std::endl;
    }*/

    return 0;
}

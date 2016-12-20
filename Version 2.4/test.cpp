#include "cath_ADA.h"

int main(int argc, char ** argv)
{
    cath::ADA a("examples/ez.ada", false);
    //cath::ADA b("examples/abp_error2_complement.ada", false);
    //a.is_empty_abstract_mode(true);
    //std::cout << a.complement() << std::endl;
    if(a.complement().is_empty_abstract_mode(true))
        std::cout << "EMPTY" << std::endl;

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

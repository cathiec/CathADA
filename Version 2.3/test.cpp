#include "cath_ADA.h"

int main(int argc, char ** argv)
{
    cath::ADA a("example3.ada", false);
    cath::ADA b("example2.ada", false);

    cath::ADA c = a.intersect(a.complement());

    try{
        z3::expr init = cath::parse("(and q0 q0b)");
        if(c.is_empty_abstract_mode(init, true))
            std::cout << "EMPTY" << std::endl;
        else
            std::cout << "NOT EMPTY" << std::endl;
    }catch(z3::exception e)
    {
        std::cout << e << std::endl;
    }

    return 0;
}

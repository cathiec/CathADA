#include "cath_ADA.h"

int main(int argc, char ** argv)
{
    cath::ADA a("examples/example1.ada", false);
    cath::ADA b("examples/example2.ada", false);

    cath::ADA c = a.intersect(b.complement());
    std::cout << c << std::endl;

    try{
        z3::expr init = cath::parse("(and q0 q0_new)");
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

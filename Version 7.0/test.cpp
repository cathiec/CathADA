#include "cath_ADA.h"

int main(int argc, char ** argv)
{
    cath::declare("q0", cath::_BOOL, true);
    cath::declare("q1", cath::_BOOL, true);
    cath::declare("q2", cath::_BOOL, true);
    cath::declare("x0", cath::_INT, true);
    cath::declare("x1", cath::_INT, true);
    z3::expr e = cath::parse("(not (not q1))");
    std::cout << e << std::endl;
    std::cout << "###" << std::endl;
    std::vector<z3::expr> temp = cath::pick_states(e);
    for(int i = 0; i < temp.size(); i++)
        std::cout << temp[i] << std::endl;

    return 0;
}

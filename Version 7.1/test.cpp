#include "cath_ADA.h"

int main(int argc, char ** argv)
{
    cath::declare("q0", cath::_BOOL, true);
    cath::declare("q1", cath::_BOOL, true);
    cath::declare("q2", cath::_BOOL, true);
    cath::declare("x0", cath::_INT, true);
    cath::declare("x1", cath::_INT, true);
    z3::expr e = cath::parse("(not (not q1))");
    z3::expr temp = cath::time_stamp(e, 7);
    std::cout << temp << std::endl;
    std::cout << cath::remove_stamp(temp) << std::endl;

    return 0;
}

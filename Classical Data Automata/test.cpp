#include "DA.h"

int main(int argc, char ** argv)
{
    cath::DA da;

    cath::declare("L", cath::_INT, true);
    cath::declare("old", cath::_INT, true);
    cath::declare("new", cath::_INT, true);

    da.i = cath::state("q0");
    da.F.push_back(cath::state("q2"));
    da.SIGMA.push_back(std::make_tuple("q0", "a", "q1", "( = L 0)"));
    da.SIGMA.push_back(std::make_tuple("q1", "a", "q2", "( != L 0)"));
    da.SIGMA.push_back(std::make_tuple("q0", "a", "q1", "( = L 0)"));
    da.SIGMA.push_back(std::make_tuple("q0", "a", "q1", "( = L 0)"));
    da.SIGMA.push_back(std::make_tuple("q0", "a", "q1", "( = L 0)"));
    da.SIGMA.push_back(std::make_tuple("q0", "a", "q1", "( = L 0)"));
    da.SIGMA.push_back(std::make_tuple("q0", "a", "q1", "( = L 0)"));

    da.emptiness();

    return 0;
}

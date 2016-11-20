#include "cath_ADA.h"

int main(int argc, char ** argv)
{
    //cath::expression e("|(&(#a, >(x, 5)), &(#b, <(x, 7)))");
    //cath::expression e("|(&(#q1,>(x,5),=(x',-(x,1))),&(#q2,<=(x,5),=(x',+(x,2))))");
    cath::expression e("%(x,x',|(&(#q1,>(x,5),=(x',-(x,1))),&(#q2,<=(x,5),=(x',+(x,2)))))");

    std::cout << e.to_string() << std::endl;

    z3::solver s(cath::z3_context);
    s.add(e.z3());
    std::cout << s << std::endl << s.check() << std::endl;
	return 0;
}

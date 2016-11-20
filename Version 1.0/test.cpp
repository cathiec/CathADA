#include "cath_ADA.h"

int main(int argc, char ** argv)
{
    cath::expression e("&(@(x,>(x,5)),%(x,<(x,7)))");

    std::cout << e.to_string() << std::endl;

    z3::solver s(cath::z3_context);
    s.add(e.z3());
    std::cout << s << std::endl << s.check() << std::endl;
	return 0;
}

#include "cath_ADA.h"

int main(int argc, char ** argv)
{
    /*cath::expression e("&(@(x,>(x,5)),%(x,<(x,x')))");
    e.set_step(10);

    std::cout << e.to_string() << std::endl;

    z3::solver s(cath::z3_context);
    s.add(e.z3());
    std::cout << s << std::endl << s.check() << std::endl;*/

    cath::ADA A("example1");

    cath::expression p1 = A.post("#q0", "a");
    std::cout << p1.to_string() << std::endl;
    cath::expression p2 = A.post(p1, "b");
    std::cout << p2.to_string() << std::endl;
    cath::expression p3 = A.post(p2, "c");
    std::cout << p3.to_string() << std::endl;

    z3::solver s(cath::z3_context);
    s.add(p3.z3());
    std::cout << s << std::endl << s.check() << std::endl;

	return 0;
}

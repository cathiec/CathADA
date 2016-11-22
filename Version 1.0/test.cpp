#include "cath_ADA.h"

int main(int argc, char ** argv)
{
    cath::ADA A("example1");

    /*
    if(A.is_empty(cath::expression("#q0")))
        std::cout << "EMPTY" << std::endl;
    else
        std::cout << "NOT EMPTY" << std::endl;
    */

    /*
    cath::expression init = cath::expression("#q0");
    cath::expression p1 = A.post("#q0", "a");
    //std::cout << p1.to_string() << std::endl;
    cath::expression p2 = A.post(p1, "b");
    //std::cout << p2.to_string() << std::endl;
    cath::expression p3 = A.post(p2, "c");

    //std::cout << p3.to_string() << std::endl;
    //z3::expr p = p3.reverse_copy().z3();
    z3::expr p = p3.z3();
    std::cout << p << std::endl;

    z3::solver s(cath::z3_context);
    s.add(p);
    std::cout << s.check() << std::endl;
    if(s.check() == z3::sat)
    {
        std::cout << "proved" << "\n";
        std::cout << "example:\n" << s.get_model() << "\n";
    }
    else
    {
        std::cout << "failed to prove" << "\n";
        std::cout << "counter-example:\n" << s.get_model() << "\n";
    }
    */

    ///*
    cath::expression init = cath::expression("#q0");
    cath::expression p1 = A.post(init, "a");
    //std::cout << p1.to_string() << std::endl;
    cath::expression p2 = A.post(p1, "b");
    //std::cout << p2.to_string() << std::endl;
    cath::expression p3 = A.post(p2, "c");

    cath::expression e("&(|(&(|(#a,#b),#c),#d),#e)");
    std::cout << p2.z3() << std::endl;
    std::cout << p2.to_string() << std::endl;
    std::cout << p2.DNF().to_string() << std::endl;
    //*/

	return 0;
}

#include "cath_ADA.h"

int main(int argc, char ** argv)
{
    /*
    cath::expression e1("&(&(<=(x, 8), =(x', +(x, 1))), &(>(x, 3), =(x', +(x, 1))))");
    cath::expression e2("&(&(>=(x, 6), =(x', +(x, 1))), &(<=(x, 7), =(x', +(x, 1))))");
    cath::expression e3("&(>(x, 8), <(x, 10))");
    e1.set_step(1);
    e2.set_step(2);
    e3.set_step(3);
    std::cout << e1.to_string() << std::endl;
    std::cout << e2.to_string() << std::endl;
    std::cout << e3.to_string() << std::endl;
    z3::expr i1 = cath::compute_interpolant(e1.z3(), (e2 * e3).z3());
    z3::expr i2 = cath::compute_interpolant(i1 && e2.z3(), e3.z3());
    std::cout << cath::interpolant(i1).to_string() << std::endl;
    std::cout << cath::interpolant(i2).to_string() << std::endl;
    //*/

    ///*
    cath::ADA A("example1");
    if(A.is_empty(cath::expression("#q0")))
        std::cout << "EMPTY" << std::endl;
    else
        std::cout << "NOT EMPTY" << std::endl;
    //*/

	return 0;
}

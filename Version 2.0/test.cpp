#include "cath_ADA.h"

int main(int argc, char ** argv)
{
    ///*
    cath::ADA a("example4.ada");

    if(a.is_empty(cath::parse("q0"), cath::ABSTRACT, true))
        std::cout << "EMPTY" << std::endl;
    else
        std::cout << "NOT EMPTY" << std::endl;
    //*/

    /*
    cath::declare("x", cath::_INT);
    cath::declare("q1", cath::_BOOL);
    cath::declare("q2", cath::_BOOL);
    cath::declare("q3", cath::_BOOL);
    std::cout << std::endl;

    cath::expression e1 = cath::context.parse_string("(assert (and q1 q2))", cath::_DEFAULT_SORTS, cath::_DECLS);
    std::cout << e1 << std::endl;
    std::cout << e1.MAIN() << std::endl;
    std::cout << std::endl;

    z3::expr_vector v1(cath::context), v2(cath::context);
    v1.push_back(cath::bool_const("q1")._z3);
    v2.push_back(cath::bool_const("q3")._z3);
    v1.push_back(cath::bool_const("q2").MAIN()._z3);
    v2.push_back(cath::expression("(< x 3)").MAIN()._z3);
    cath::expression e2 = e1._z3.substitute(v1, v2);
    std::cout << e2 << std::endl;
    std::cout << e2.MAIN() << std::endl;
    //*/

    return 0;
}

#include "cath_ADA.h"

int main(int argc, char ** argv)
{
    cath::ADA a("examples/array_shift1.ada", false);
    cath::ADA b("examples/array_shift2.ada", false);
    a.intersect(b.complement());

    z3::expr e1 = cath::parse("(and q1 (< i0 3))") || cath::parse("(and q2 (> j0 7))");
    z3::expr e2 = cath::parse("(or q1_new (< i1 3))") && cath::parse("(or q2_new (< j1 3))");
    z3::expr e = cath::parse("(< a0 3)") && cath::parse("(> b0 4)");
    z3::expr_vector from(cath::context), to(cath::context);
    from.push_back(cath::parse("(< a0 3)"));
    from.push_back(cath::parse("(> b0 4)"));
    to.push_back(e1);
    to.push_back(e2);
    std::cout << e.substitute(from, to) << std::endl;
    std::cout << std::endl;
    std::vector<z3::expr> v = cath::DNF_array(e.substitute(from, to));
    for(int i = 0; i < v.size(); i++)
        std::cout << v[i] << std::endl;

    /*cath::ADA c = a.intersect(a.complement());
    std::cout << c << std::endl;

    try{
        if(c.is_empty_abstract_mode(true))
            std::cout << "EMPTY" << std::endl;
        else
            std::cout << "NOT EMPTY" << std::endl;
    }catch(z3::exception e)
    {
        std::cout << e << std::endl;
    }*/

    return 0;
}

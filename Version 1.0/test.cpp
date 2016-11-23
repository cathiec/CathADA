#include "cath_ADA.h"

int main(int argc, char ** argv)
{
    cath::ADA A("example1");

    std::vector<cath::expression> I;
    I.push_back(cath::expression("$false"));
    I.push_back(cath::expression(">=(x,5)"));
    I.push_back(cath::expression("<=(x,9)"));
    I.push_back(cath::expression(">=(x,6)"));
    I.push_back(cath::expression("<=(x,10)"));
    I.push_back(cath::expression(">=(x,7)"));
    I.push_back(cath::expression("<=(x,8)"));
    //if(A.is_empty2(cath::expression("#q0")))
    if(A.is_empty(cath::expression("#q0"), I))
        std::cout << "EMPTY" << std::endl;
    else
        std::cout << "NOT EMPTY" << std::endl;

    /*
    cath::expression init = cath::expression("#q0");
    cath::expression p1 = A.post(init, "a");
    std::cout << p1.to_string() << std::endl;
    cath::expression p2 = A.post(p1, "b");
    std::cout << p2.to_string() << std::endl;
    cath::expression p3 = A.post(p2, "c");
    std::cout << p3.to_string() << std::endl;
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

    /*
    std::vector<cath::expression> I;
    I.push_back(cath::expression(">=(x,5)"));
    I.push_back(cath::expression("<=(x,9)"));
    I.push_back(cath::expression(">=(x,6)"));
    I.push_back(cath::expression("<=(x,10)"));
    I.push_back(cath::expression(">=(x,7)"));
    I.push_back(cath::expression("<=(x,8)"));
    cath::expression init = cath::expression("#q0");
    cath::expression p1 = A.abstract_post(init, "a", I);
    std::cout << p1.to_string() << std::endl;
    std::cout << std::endl;
    cath::expression p2 = A.abstract_post(p1, "b", I);
    std::cout << p2.to_string() << std::endl;
    std::cout << std::endl;
    cath::expression p3 = A.abstract_post(p2, "d", I);
    std::cout << p3.to_string() << std::endl;
    std::cout << std::endl;
    cath::expression p4 = A.abstract_post(p3, "d", I);
    std::cout << p4.to_string() << std::endl;
    std::cout << std::endl;
    cath::expression p5 = A.abstract_post(p4, "d", I);
    std::cout << p5.to_string() << std::endl;

    //cath::expression e("&(|(&(|(#a,#b),#c),#d),#e)");
    //std::cout << p3.z3() << std::endl;
    //std::cout << p1.to_string() << std::endl << std::endl;
    //std::cout << "DNF: " << p1.DNF().to_string() << std::endl << std::endl;
    //std::cout << p2.to_string() << std::endl << std::endl;
    //std::cout << "DNF: " << p2.DNF().to_string() << std::endl << std::endl;
    //std::cout << p3.to_string() << std::endl << std::endl;
    //std::cout << "DNF: " << p3.DNF().to_string() << std::endl;
    */

	return 0;
}

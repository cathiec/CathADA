#include "cath_ADA.h"
#include "src/timer.h"

int main(int argc, char ** argv)
{
    z3::expr e = cath::parse("(let (xxx (> 3 2))) (xxx)");
    std::cout << e << std::endl;

    return 0;
}

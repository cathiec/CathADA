#include "cath_ADA.h"
#include "src/timer.h"

// argv[1] automaton 1
// argv[2] automaton 2
// argv[3+]
    // -print : print out all the steps

int main(int argc, char ** argv)
{
    cath::ADA a(argv[1]);
    cath::ADA b(argv[2]);

    cath::ADA result = a.intersect(b.complement());
    std::ofstream file("result.ada");
    file << result << std::endl;

    return 0;
}

#include "cath_ADA.h"
#include "src/timer.h"

// argv[1] automaton 1
// argv[2] automaton 2
// argv[3+]
    // -print : print out all the steps

int main(int argc, char ** argv)
{
    bool print = false;

    for(int i = 3; i < argc; i++)
    {
        std::string temp = argv[i];
        if(temp == "-print")
            print = true;
    }

    cath::ADA a(argv[1], print);
    cath::ADA b(argv[2], print);

    cath::time_point start, end;

    start = cath::now();

    if(a.intersect(b.complement()).is_empty(cath::ABSTRACT, print))
        std::cout << "L(1) <= L(2)." << std::endl;
    else
        std::cout << "Not included." << std::endl;

    end = cath::now();

    double time_sec = cath::time_diff_sec(start, end);
    double time_msec = cath::time_diff_msec(start, end);
    unsigned long time_usec = cath::time_diff_usec(start, end);

    printf("\t|\t-> Time used: %.3lf sec\n", time_sec);
    printf("\t|\t-> Time used: %.3lf msec\n", time_msec);
    printf("\t|\t-> Time used: %ld usec\n", time_usec);

    return 0;
}

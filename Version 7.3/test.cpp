#include "cath_ADA.h"
#include "src/timer.h"

int main(int argc, char ** argv)
{
    cath::ADA a("examples/example1.ada", true);

    cath::time_point start, end;

    start = cath::now();

    if(a.is_empty(true))
        std::cout << "EMPTY" << std::endl;
    else
        std::cout << "NOT EMPTY" << std::endl;

    end = cath::now();

    double time_sec = cath::time_diff_sec(start, end);
    double time_msec = cath::time_diff_msec(start, end);
    unsigned long time_usec = cath::time_diff_usec(start, end);

    printf("\t|\t-> Time used: %.3lf sec\n", time_sec);
    printf("\t|\t-> Time used: %.3lf msec\n", time_msec);
    printf("\t|\t-> Time used: %ld usec\n", time_usec);

    return 0;
}

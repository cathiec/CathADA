#ifndef _timer_h
#define _timer_h

#include <sys/time.h>

namespace cath
{

    typedef struct
    {
        unsigned long sec;
        unsigned long usec;
    }time_point;

    time_point now()
    {
        time_point result;
        struct timeval tv;
        gettimeofday(&tv,NULL);
        result.sec = tv.tv_sec;
        result.usec = tv.tv_usec;
        return result;
    }

    unsigned long time_diff_usec(const time_point & t1, const time_point & t2)
    {
        unsigned long result = (t2.sec - t1.sec) * 1000000;
        result += (t2.usec - t1.usec);
        return result;
    }

    double time_diff_msec(const time_point & t1, const time_point & t2)
    {
        return time_diff_usec(t1, t2) / 1000.0;
    }

    double time_diff_sec(const time_point & t1, const time_point & t2)
    {
        return time_diff_msec(t1, t2) / 1000.0;
    }

};

#endif

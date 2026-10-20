#ifndef CTIMER_UTILS_H
#define CTIMER_UTILS_H

#include <time.h>

struct timespec tv1{}, tv2{};

#define TIMER_CLEAR \
    do { tv1 = {}; tv2 = {}; } while (0)

#define TIMER_START \
    timespec_get(&tv1, TIME_UTC)

#define TIMER_ELAPSED \
    (difftime(tv2.tv_sec, tv1.tv_sec) + \
     ((double)tv2.tv_nsec - (double)tv1.tv_nsec) / 1e9)

#define TIMER_STOP \
    timespec_get(&tv2, TIME_UTC)

#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "random.h"

typedef unsigned short xsubi[3];

static xsubi *states;

void initRandom(unsigned long seed)
{
    if (seed == 0)
        srand48((unsigned long)time(NULL));
    else
        srand48(seed);

    int maxThreads = 1;

#ifdef _OPENMP
    maxThreads = omp_get_max_threads();
#endif

    states = (xsubi *) malloc(sizeof(xsubi) * maxThreads);

    for (int i = 0; i < maxThreads; i++)
    {
        states[i][0] = (unsigned short) (0xFFFF * drand48());
        states[i][1] = (unsigned short) (0xFFFF * drand48());
        states[i][2] = (unsigned short) (0xFFFF * drand48());
    }
}

double drandom(void)
{
    int threadId = 0;

#ifdef _OPENMP
    threadId = omp_get_thread_num();
#endif

    return erand48(states[threadId]);
}

int irandom(int limit)
{
    return ((int) (0xFFFFFF * drandom())) % limit;
}

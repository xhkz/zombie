#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#include "random.h"

typedef unsigned short xsubi[3];

static xsubi *states;

/* Make sure different processor gets different seed */
void initRandom(unsigned long seed, int rank)
{
    if (seed == 0)
        srand48((unsigned long)time(NULL) + rank);
    else
        srand48(seed + rank);

    int maxThreads = omp_get_max_threads();

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
    int threadId = omp_get_thread_num();

    return erand48(states[threadId]);
}

int irandom(int limit)
{
    return ((int) (0xFFFFFF * drandom())) % limit;
}

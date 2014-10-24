#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "constants.h"
#include "entity.h"
#include "utils.h"
#include "matrix.h"
#include "random.h"

int benchmark = false;

#ifdef _OPENMP
void lock(int i, bool *locks)
{
    for (bool locked = false; locked == false; /*NOP*/)
    {
        #pragma omp critical (LockRegion)
        {
            locked = !locks[i-1] && !locks[i] && !locks[i+1];
            if (locked)
            {
                locks[i-1] = true;
                locks[i] = true;
                locks[i+1] = true;
            }
        }
    }
}

void unlock(int i, bool *locks)
{
    #pragma omp critical (LockRegion)
    {
        locks[i-1] = false;
        locks[i] = false;
        locks[i+1] = false;
    }
}
#endif


int main(int argc, char **argv)
{
    initRandom(0);

    bool debug = false;
    int threadsLimit = 0;

    int c;
    while ((c = getopt (argc, argv, "dn:t")) != -1)
    {
        switch (c)
        {
        case 'd':
            debug = true;
            break;
        case 'n':
            threadsLimit = atoi(optarg);
            break;
        case 't':
            benchmark = true;
            break;
        default:
            ;
        }
    }
    
    if (threadsLimit) {
        omp_set_dynamic(0);
        omp_set_num_threads(threadsLimit);
    }
    
    clock_t start;
    start = clock();

    bool *locks = (bool *)malloc((SIZEX + 2) * sizeof(bool));

    for (int i = 0; i < SIZEX + 2; i++)
        locks[i] = false;

    Entity **matrix_a = createMatrix(SIZEX + 2, SIZEY + 2);
    Entity **matrix_b = createMatrix(SIZEX + 2, SIZEY + 2);

    initMatrix(matrix_a, SIZEX, SIZEY);

    if (!benchmark) {
        update_counter(matrix_a);
        print_header();
        print_csv(0);
    }

    for (int n = 0; n < STEPS; n++)
    {
#ifdef _OPENMP
        #pragma omp parallel for default(none) shared(matrix_a, matrix_b, n, locks, debug) schedule(static, SIZEX/omp_get_max_threads())
#endif
        for (int i = 1; i <= SIZEX; i++)
        {
#ifdef _OPENMP
            lock(i, locks);
#endif
            if (debug)
                printf("step: %d, index_x: %d, thread_id: %d\n", n, i, omp_get_thread_num());
            #pragma omp parallel for
            for (int j = 1; j <= SIZEY; j++)
            {
                process(matrix_a, matrix_b, i, j);
            }
#ifdef _OPENMP
            unlock(i, locks);
#endif
        }
        transferInBorder(matrix_a, matrix_b);
        moveBackInBorder(matrix_b);

        //swap matrix
        Entity **matrix_t = matrix_a;
        matrix_a = matrix_b;
        matrix_b = matrix_t;

        if (!benchmark) {
            update_counter(matrix_a);
            print_csv(n+1);
        }
    }
    
    if (benchmark)
        printf("Thread: %d, Time: %f sec\n", omp_get_max_threads(), (double)(clock() - start) / CLOCKS_PER_SEC);

    destroyMatrix(matrix_a);
    destroyMatrix(matrix_b);

    return 0;
}

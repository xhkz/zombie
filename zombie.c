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

    int c;
    while ((c = getopt (argc, argv, "d")) != -1)
    {
        switch (c)
        {
        case 'd':
            debug = true;
            break;
        default:
            ;
        }
    }

    bool *locks = (bool *)malloc((SIZEX + 2) * sizeof(bool));

    for (int i = 0; i < SIZEX + 2; i++)
        locks[i] = false;

    Entity **matrix_a = createMatrix(SIZEX + 2, SIZEY + 2);
    Entity **matrix_b = createMatrix(SIZEX + 2, SIZEY + 2);

    initMatrix(matrix_a, SIZEX, SIZEY);

    print_population(matrix_a, 0);

    for (int n = 0; n < STEPS; n++)
    {
#ifdef _OPENMP
        #pragma omp parallel for default(none) shared(matrix_a, matrix_b, n, locks, debug)
#endif
        for (int i = 1; i <= SIZEX; i++)
        {
#ifdef _OPENMP
            lock(i, locks);
#endif
            if (debug)
                printf("step: %d, index_x: %d, thread_id: %d\n", n, i, omp_get_thread_num());

            for (int j = 1; j <= SIZEY; j++)
            {
                process(matrix_a, matrix_b, i, j);
            }
#ifdef _OPENMP
            unlock(i, locks);
#endif
        }
        moveBackInBorder(matrix_b);

        //swap matrix
        Entity **matrix_t = matrix_a;
        matrix_a = matrix_b;
        matrix_b = matrix_t;

        print_population(matrix_a, n+1);
        //print_matrix(matrix_a, n+1);
    }

    return 0;
}

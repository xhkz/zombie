#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "constants.h"
#include "entity.h"
#include "utils.h"
#include "matrix.h"

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
    srand48(8767134);
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
        #pragma omp parallel for default(none) shared(matrix_a,matrix_b,n,locks)
        #endif
        for (int i = 1; i <= SIZEX; i++)
        {
            #ifdef _OPENMP
            lock(i, locks);
            #endif
            for (int j = 1; j <= SIZEY; j++)
            {
                Entity *p = &matrix_a[i][j];

                if (p->type != EMPTY)
                {
                    p->type = EMPTY;
                    double move = drand48();
                    double moveChance = p->moveChance;

                    if (move < 1.0*moveChance && matrix_a[i-1][j].type == EMPTY && matrix_b[i-1][j].type == EMPTY)
                    {
                        matrix_b[i-1][j].type = matrix_a[i][j].type;
                    }
                    else if (move < 2.0*moveChance && matrix_a[i+1][j].type == EMPTY && matrix_b[i+1][j].type == EMPTY)
                    {
                        matrix_b[i+1][j].type = matrix_a[i][j].type;
                    }
                    else if (move < 3.0*moveChance && matrix_a[i][j-1].type == EMPTY && matrix_b[i][j-1].type == EMPTY)
                    {
                        matrix_b[i][j-1].type = matrix_a[i][j].type;
                    }
                    else if (move < 4.0*moveChance && matrix_a[i][j+1].type == EMPTY && matrix_b[i][j+1].type == EMPTY)
                    {
                        matrix_b[i][j+1].type = matrix_a[i][j].type;
                    }
                    else
                    {
                        matrix_b[i][j].type = matrix_a[i][j].type;
                    }
                }
            }

            // MOVE BACK
            #ifdef _OPENMP
            unlock(i, locks);
            #endif
        }

        // move point out of boundary back to matrix

        Entity **matrix_t = matrix_a;
        matrix_a = matrix_b;
        matrix_b = matrix_t;

        print_population(matrix_a, n+1);
        print_matrix(matrix_a, n+1);
    }

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#include <omp.h>
#include <mpi.h>

#include "constants.h"
#include "entity.h"
#include "utils.h"
#include "matrix.h"
#include "random.h"

int main(int argc, char **argv)
{
    bool *locks = (bool *)malloc((SIZEX + 2) * sizeof(bool));
    for (int i = 0; i < SIZEX + 2; i++)
        locks[i] = false;

    int rank, size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    initRandom(0, rank);
    Entity **matrix_a = createMatrix(SIZEX + 2, SIZEY + 2);
    Entity **matrix_b = createMatrix(SIZEX + 2, SIZEY + 2);
    initMatrix(matrix_a, SIZEX, SIZEY);

    MPI_Datatype cell_t;
    MPI_Type_contiguous(sizeof(Entity), MPI_BYTE, &cell_t);
    MPI_Type_commit(&cell_t);

    MPI_Datatype row_t;
    MPI_Type_vector(SIZEX + 2, 1, 1, cell_t, &row_t);
    MPI_Type_commit(&row_t);

    MPI_Datatype counter_t;
    MPI_Type_contiguous(sizeof(Counter), MPI_BYTE, &counter_t);
    MPI_Type_commit(&counter_t);

    Entity * northBuffer = (Entity *) malloc((SIZEX + 2) * sizeof(Entity));
    Entity * southBuffer = (Entity *) malloc((SIZEX + 2) * sizeof(Entity));
    Counter counterBuffer;

    updateCounter(matrix_a);

    if (rank == NORTH)
    {
        MPI_Recv(&counterBuffer, 1, counter_t, SOUTH, TAG, MPI_COMM_WORLD, &status);
        mergeCounter(counterBuffer);
    }
    else
        MPI_Send(&counter, 1, counter_t, NORTH, TAG, MPI_COMM_WORLD);

    if (rank == NORTH) {
        printHeader();
        printCSV(0);
    }

    for (int n = 0; n < STEPS; n++)
    {
        // set adjacent borders
        if (rank == NORTH)
        {
            MPI_Recv(northBuffer, 1, row_t, SOUTH, TAG, MPI_COMM_WORLD, &status);
            #pragma omp parallel for
            for (int i = 0; i < SIZEX + 2; i++)
                copyEntity(&northBuffer[i], &matrix_a[i][SIZEY+1]);

            #pragma omp parallel for
            for (int i = 0; i < SIZEX + 2; i++)
                copyEntity(&matrix_a[i][SIZEY], &northBuffer[i]);
            MPI_Send(northBuffer, 1, row_t, SOUTH, TAG, MPI_COMM_WORLD);
        }

        if (rank == SOUTH)
        {
            #pragma omp parallel for
            for (int i = 0; i < SIZEX + 2; i++)
                copyEntity(&matrix_a[i][1], &southBuffer[i]);
            MPI_Send(southBuffer, 1, row_t, NORTH, TAG, MPI_COMM_WORLD);

            MPI_Recv(southBuffer, 1, row_t, NORTH, TAG, MPI_COMM_WORLD, &status);
            #pragma omp parallel for
            for (int i = 0; i < SIZEX + 2; i++)
                copyEntity(&southBuffer[i], &matrix_a[i][0]);
        }

        #pragma omp parallel for default(none) shared(matrix_a, matrix_b, n, locks) schedule(static, SIZEX/omp_get_max_threads())
        for (int i = 1; i <= SIZEX; i++)
        {
            lock(i, locks);
            #pragma omp parallel for
            for (int j = 1; j <= SIZEY; j++)
                process(matrix_a, matrix_b, i, j);

            unlock(i, locks);
        }

        // merge adjacent border
        if (rank == NORTH)
        {
            MPI_Recv(northBuffer, 1, row_t, SOUTH, TAG, MPI_COMM_WORLD, &status);
            int i = 0, j = 0;
            while (i < SIZEX + 2) {
                if (northBuffer[i].type != EMPTY) {
                    for (j = i; j < SIZEX + 2; j++) {
                        if (matrix_b[j][SIZEY].type == EMPTY) {
                            copyEntity(&northBuffer[i], &matrix_b[j][SIZEY]);
                            // printf("merged S[%d] to N[%d]\n", i, j);
                            break;
                        }
                    }

                    if (j == SIZEX + 2)
                        break;
                }
                i++;
            }

            #pragma omp parallel for
            for (int i = 0; i < SIZEX + 2; i++) {
                copyEntity(&matrix_b[i][SIZEY+1], &northBuffer[i]);
                clearEntity(&matrix_b[i][SIZEY+1]);
            }
            MPI_Send(northBuffer, 1, row_t, SOUTH, TAG, MPI_COMM_WORLD);
        }

        if (rank == SOUTH)
        {
            #pragma omp parallel for
            for (int i = 0; i < SIZEX + 2; i++) {
                copyEntity(&matrix_b[i][0], &southBuffer[i]);
                clearEntity(&matrix_b[i][0]);
            }
            MPI_Send(southBuffer, 1, row_t, NORTH, TAG, MPI_COMM_WORLD);

            MPI_Recv(southBuffer, 1, row_t, NORTH, TAG, MPI_COMM_WORLD, &status);
            int i = 0, j = 0;
            while (i < SIZEX + 2) {
                if (southBuffer[i].type != EMPTY) {
                    for (j = i; j < SIZEX + 2; j++) {
                        if (matrix_b[j][1].type == EMPTY) {
                            copyEntity(&southBuffer[i], &matrix_b[j][1]);
                            // printf("merged N[%d] to S[%d]\n", i, j);
                            break;
                        }
                    }

                    if (j == SIZEX + 2)
                        break;
                }
                i++;
            }
        }

        // clear original adjacent border in matrix_a
        for (int i = 0; i < SIZEX + 2; i++)
            clearEntity(&matrix_a[i][rank == NORTH ? SIZEY + 1 : 0]);

        //some times it can not move back, then stay in the border
        transferInBorder(matrix_a, matrix_b);
        moveBackInBorder(matrix_b);
        Entity **matrix_t = matrix_a;
        matrix_a = matrix_b;
        matrix_b = matrix_t;
        updateCounter(matrix_a);

        if (rank == NORTH)
        {
            MPI_Recv(&counterBuffer, 1, counter_t, SOUTH, TAG, MPI_COMM_WORLD, &status);
            mergeCounter(counterBuffer);
        }
        else
            MPI_Send(&counter, 1, counter_t, NORTH, TAG, MPI_COMM_WORLD);

        if (rank == NORTH) {
            printCSV(n+1);
        }
    }

    destroyMatrix(matrix_a);
    destroyMatrix(matrix_b);

    MPI_Finalize();

    return 0;
}

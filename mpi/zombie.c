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


/* Type and Buffer */
int rank, size;
MPI_Datatype cell_t;
MPI_Datatype row_t;
MPI_Datatype counter_t;
MPI_Status   status;
Counter counterBuffer;

bool benchmark = false;

void syncCounter()
{
    if (rank == ROOT)
    {
        MPI_Recv(&counterBuffer, 1, counter_t, SOUTH, TAG, MPI_COMM_WORLD, &status);
        mergeCounter(counterBuffer);
    }
    else
    {
        MPI_Send(&counter, 1, counter_t, NORTH, TAG, MPI_COMM_WORLD);
    }
}

void setBorder(Entity * buffer, Entity ** matrix, int y)
{
    #pragma omp parallel for
    for (int i = 0; i < SIZEX + 2; i++)
        copyEntity(&buffer[i], &matrix[i][y]);
}

void setBuffer(Entity ** matrix, Entity * buffer, int y)
{
    #pragma omp parallel for
    for (int i = 0; i < SIZEX + 2; i++)
        copyEntity(&matrix[i][y] , &buffer[i]);
}

void mergeGhost(Entity * buffer, Entity ** matrix, int y)
{
    int i = 0, j = 0;
    while (i < SIZEX + 2)
    {
        if (buffer[i].type != EMPTY)
        {
            for (j = i; j < SIZEX + 2; j++)
            {
                if (matrix[j][y].type == EMPTY)
                {
                    copyEntity(&buffer[i], &matrix[j][y]);
                    // printf("{%d} merged [%d] to [%d]\n", rank, i, j);
                    break;
                }
            }

            // no space from i to right end
            if (j == SIZEX + 2)
                break;
        }
        i++;
    }
}

void setGhost(Entity ** matrix, Entity * buffer, int y)
{
    #pragma omp parallel for
    for (int i = 0; i < SIZEX + 2; i++)
    {
        copyEntity(&matrix[i][y], &buffer[i]);
        clearEntity(&matrix[i][y]);
    }
}

int main(int argc, char **argv)
{

    int threadsLimit = 0;

    int c;
    while ((c = getopt (argc, argv, "n:t")) != -1)
    {
        switch (c)
        {
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

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    initRandom(0, rank);
    Entity **matrix_a = createMatrix(SIZEX + 2, SIZEY + 2);
    Entity **matrix_b = createMatrix(SIZEX + 2, SIZEY + 2);
    initMatrix(matrix_a, SIZEX, SIZEY);

    MPI_Type_contiguous(sizeof(Entity), MPI_BYTE, &cell_t);
    MPI_Type_commit(&cell_t);
    MPI_Type_vector(SIZEX + 2, 1, 1, cell_t, &row_t);
    MPI_Type_commit(&row_t);
    MPI_Type_contiguous(sizeof(Counter), MPI_BYTE, &counter_t);
    MPI_Type_commit(&counter_t);

    Entity * northBuffer = (Entity *) malloc((SIZEX + 2) * sizeof(Entity));
    Entity * southBuffer = (Entity *) malloc((SIZEX + 2) * sizeof(Entity));

    if (!benchmark) {
        // update local counter and sync
        updateCounter(matrix_a);
        syncCounter();
        printHeader(rank);
        printCSV(0, rank);
    }

    for (int n = 0; n < STEPS; n++)
    {
        // set adjacent borders
        if (rank == NORTH)
        {
            MPI_Recv(northBuffer, 1, row_t, SOUTH, TAG, MPI_COMM_WORLD, &status);
            setBorder(northBuffer, matrix_a, SIZEY+1);

            setBuffer(matrix_a, northBuffer, SIZEY);
            MPI_Send(northBuffer, 1, row_t, SOUTH, TAG, MPI_COMM_WORLD);
        }

        if (rank == SOUTH)
        {
            setBuffer(matrix_a, southBuffer, 1);
            MPI_Send(southBuffer, 1, row_t, NORTH, TAG, MPI_COMM_WORLD);

            MPI_Recv(southBuffer, 1, row_t, NORTH, TAG, MPI_COMM_WORLD, &status);
            setBorder(southBuffer, matrix_a, 0);
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
            mergeGhost(northBuffer, matrix_b, SIZEY);

            setGhost(matrix_b, northBuffer, SIZEY+1);
            MPI_Send(northBuffer, 1, row_t, SOUTH, TAG, MPI_COMM_WORLD);
        }

        if (rank == SOUTH)
        {
            setGhost(matrix_b, southBuffer, 0);
            MPI_Send(southBuffer, 1, row_t, NORTH, TAG, MPI_COMM_WORLD);

            MPI_Recv(southBuffer, 1, row_t, NORTH, TAG, MPI_COMM_WORLD, &status);
            mergeGhost(southBuffer, matrix_b, 1);
        }

        // clear original adjacent border in matrix_a
        for (int i = 0; i < SIZEX + 2; i++)
            clearEntity(&matrix_a[i][rank == NORTH ? SIZEY + 1 : 0]);

        //some times it can not move back, then stay in the border
        transferInBorder(matrix_a, matrix_b);
        moveBackInBorder(matrix_b);

        // swap matrixes
        Entity **matrix_t = matrix_a;
        matrix_a = matrix_b;
        matrix_b = matrix_t;

        if (!benchmark)
        {
            updateCounter(matrix_a);
            syncCounter();
            printCSV(n+1, rank);
        }
    }
    
    if (benchmark)
        printf("Thread: %d, Time: %f sec\n", omp_get_max_threads(), (double)(clock() - start) / CLOCKS_PER_SEC);

    destroyMatrix(matrix_a);
    destroyMatrix(matrix_b);
    
    free(northBuffer);
    free(southBuffer);

    MPI_Finalize();

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#ifdef _OPENMP
#include <omp.h>
#endif

#include "entity.h"
#include "constants.h"
#include "random.h"

Entity **createMatrix(int size_x, int size_y)
{
    Entity **matrix = (Entity **) malloc(size_y * sizeof(Entity));

    for (int i = 0; i < size_y; i++)
    {
        matrix[i] = (Entity *) malloc(size_x * sizeof(Entity));
        for (int j = 0; j < size_x; j++)
        {
            Entity entity = {EMPTY, INV, 0, NIL, 0.0, NONE, 0};
            matrix[i][j] = entity;
        }
    }

    return matrix;
}

int randomPos(int limit)
{
    return 1 + irandom(limit);
}

void initMatrix(Entity ** matrix, int size_x, int size_y)
{
    int humanCount = 0,
        zombieCount = 0,
        posX = 0,
        posY = 0;

    Entity * p = NULL;

#ifdef _OPENMP
    #pragma omp parallel private(p) shared(humanCount)
#endif
    {
        while (humanCount < INIT_HUMAN_NUM)
        {
            posX = randomPos(SIZEX);
            posY = randomPos(SIZEY);
            p = &matrix[posX][posY];

            if (p->type == EMPTY)
            {
                #pragma omp critical
                {
                    createHuman(p, NIL);
                    humanCount++;
                }
            }
        }
    }

#ifdef _OPENMP
    #pragma omp parallel private(p) shared(zombieCount)
#endif
    {
        while(zombieCount < INIT_ZOMBIE_NUM)
        {
            posX = randomPos(SIZEX);
            posY = randomPos(SIZEY);
            p = &matrix[posX][posY];

            if (p->type == EMPTY)
            {
                #pragma omp critical
                {
                    createZombie(p);
                    zombieCount++;
                }
            }
        }
    }
}

void process(Entity **matrix_a, Entity **matrix_b, int i, int j)
{
    Entity * cell_a = &matrix_a[i][j];
    Entity * cell_b = NULL;

    growup(cell_a);

    if (cell_a->type != EMPTY)
    {
        double move = drandom();
        double moveChance = cell_a->moveChance;

        randomBirth(cell_a, matrix_a, matrix_b, i, j);
        randomInfection(cell_a, matrix_a, matrix_b, i, j);

        if (move < 1.0*moveChance && matrix_a[i-1][j].type == EMPTY && matrix_b[i-1][j].type == EMPTY)
        {
            cell_b = &matrix_b[i-1][j];
        }
        else if (move < 2.0*moveChance && matrix_a[i+1][j].type == EMPTY && matrix_b[i+1][j].type == EMPTY)
        {
            cell_b = &matrix_b[i+1][j];
        }
        else if (move < 3.0*moveChance && matrix_a[i][j-1].type == EMPTY && matrix_b[i][j-1].type == EMPTY)
        {
            cell_b = &matrix_b[i][j-1];
        }
        else if (move < 4.0*moveChance && matrix_a[i][j+1].type == EMPTY && matrix_b[i][j+1].type == EMPTY)
        {
            cell_b = &matrix_b[i][j+1];
        }
        else
        {
            cell_b = &matrix_b[i][j];
        }

        if (!randomDeath(cell_a, drandom()))
        {
            cell_a->steps++;
            copyEntity(cell_a, cell_b);
        }
        clearEntity(cell_a);
    }
}

void moveBackInBorder(Entity **matrix)
{
    for (int i = 1; i <= SIZEX; i++)
    {
        moveEntity(&matrix[i][0], &matrix[i][1]);
        moveEntity(&matrix[i][SIZEY+1], &matrix[i][SIZEY]);
    }

    for (int i = 1; i <= SIZEY; i++)
    {
        moveEntity(&matrix[0][i], &matrix[1][i]);
        moveEntity(&matrix[SIZEX+1][i], &matrix[SIZEX][i]);
    }
}

void clearMatrix(Entity **matrix)
{
}

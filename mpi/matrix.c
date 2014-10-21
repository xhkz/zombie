#include <stdio.h>
#include <stdlib.h>

#include <omp.h>

#include "entity.h"
#include "constants.h"
#include "random.h"

Entity **createMatrix(int size_x, int size_y)
{
    Entity **matrix = (Entity **) malloc(size_x * sizeof(Entity *));

    for (int i = 0; i < size_x; i++)
    {
        matrix[i] = (Entity *) malloc(size_y * sizeof(Entity));
        for (int j = 0; j < size_y; j++)
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

    while (humanCount < INIT_HUMAN_NUM)
    {
        posX = randomPos(SIZEX);
        posY = randomPos(SIZEY);
        p = &matrix[posX][posY];

        if (p->type == EMPTY)
        {
            createHuman(p, NIL);
            humanCount++;
        }
    }

    while(zombieCount < INIT_ZOMBIE_NUM)
    {
        posX = randomPos(SIZEX);
        posY = randomPos(SIZEY);
        p = &matrix[posX][posY];

        if (p->type == EMPTY)
        {
            createZombie(p);
            zombieCount++;
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
        randomBirth(cell_a, matrix_a, matrix_b, i, j);
        randomInfection(cell_a, matrix_a, i, j);
        randomWalk(cell_a, &cell_b, matrix_a, matrix_b, i, j);

        if (!randomDeath(cell_a))
        {
            cell_a->steps++;
            copyEntity(cell_a, cell_b);
        }

        clearEntity(cell_a);
    }
}

void transferInBorder(Entity **matrix_a, Entity **matrix_b)
{
    for (int i = 0; i < SIZEX + 2; i++)
    {
        moveEntity(&matrix_a[i][0], &matrix_b[i][0]);
        moveEntity(&matrix_a[i][SIZEY+1], &matrix_b[i][SIZEY+1]);
    }

    for (int i = 1; i < SIZEY + 1; i++)
    {
        moveEntity(&matrix_a[0][i], &matrix_b[0][i]);
        moveEntity(&matrix_a[SIZEX+1][i], &matrix_b[SIZEX+1][i]);
    }
}

void moveBackInBorder(Entity **matrix)
{
    for (int i = 0; i < SIZEX + 2; i++)
    {
        moveEntity(&matrix[i][0], &matrix[i][1]);
        moveEntity(&matrix[i][SIZEY+1], &matrix[i][SIZEY]);
    }

    for (int i = 0; i < SIZEY + 2; i++)
    {
        moveEntity(&matrix[0][i], &matrix[1][i]);
        moveEntity(&matrix[SIZEX+1][i], &matrix[SIZEX][i]);
    }
}

void clearMatrix(Entity **matrix)
{
    for (int i = 0; i < SIZEX + 2; i++)
        for (int j = 0; j < SIZEY + 2; j++)
            clearEntity(&matrix[i][j]);
}

void destroyMatrix(Entity **matrix)
{
    for (int i = 0; i < SIZEX + 2; i++)
        free(matrix[i]);

    free(matrix);
}

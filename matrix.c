#include <stdlib.h>

#include "entity.h"
#include "constants.h"

Entity **createMatrix(int size_x, int size_y)
{
    Entity **matrix = (Entity **) malloc(size_y * sizeof(Entity));

    for (int i = 0; i < size_y; i++)
    {
        matrix[i] = (Entity *) malloc(size_x * sizeof(Entity));
        for (int j = 0; j < size_x; j++)
        {
            Entity entity = {EMPTY, INV, NIL, 0.0, NONE, 0};
            matrix[i][j] = entity;
        }
    }

    return matrix;
}

int randomPos(int limit)
{
    return 1 + (rand() % limit);
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
            createHuman(p, drand48());
            humanCount++;
        }
    }

    while (zombieCount < INIT_ZOMBIE_NUM)
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

    if (cell_a->type != EMPTY)
    {
        double move = drand48();
        double moveChance = cell_a->moveChance;

        randomBirth(cell_a, matrix_a, matrix_b, i, j);

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

        if (!randomDeath(cell_a, move))
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

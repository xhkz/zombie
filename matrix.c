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

void clearMatrix(Entity **matrix)
{
}

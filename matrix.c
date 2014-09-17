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

    double randomRate;
    Entity * p = NULL;

    while (humanCount < INIT_HUMAN_NUM)
    {
        posX = randomPos(SIZEX);
        posY = randomPos(SIZEY);
        p = &matrix[posX][posY];

        if (p->type == EMPTY)
        {
            p->type = HUMAN;
            randomRate = drand48();
            p->gender = randomRate > INIT_GENDER_RATE ? MALE : FEMALE;
            
            if(randomRate < INIT_BABY_RATE)
            {
                p->stage = BABY;
                p->moveChance = MOVE_HUMAN_BABY;
            }
            else if (randomRate < INIT_YOUNG_RATE)
            {
                p->stage = YOUNG;
                p->moveChance = MOVE_HUMAN_YOUNG;
            }
            else if (randomRate < INIT_ADULT_RATE)
            {
                p->stage = ADULT;
                p->moveChance = MOVE_HUMAN_ADULT;
            }
            else
            {
                p->stage = ELDER;
                p->moveChance = MOVE_HUMAN_ELDER;
            }

            p->status = HEALTHY;

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
            p->type = ZOMBIE;
            randomRate = drand48();
            p->gender = INV;
            p->stage = NIL;
            p->moveChance = MOVE_ZOMBIE;
            p->status = NONE;

            zombieCount++;
        }
    }
}

void clearMatrix(Entity **matrix)
{
}

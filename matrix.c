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
            Entity entity;
            entity.type = EMPTY;
            matrix[i][j] = entity;
        }
    }

    return matrix;
}

void initMatrix(Entity ** matrix, int size_x, int size_y)
{
    srand48(8767134);

    for (int i = 1; i <= size_y; i++)
    {
        for (int j = 1; j <= size_x; j++)
        {
            double chance = drand48();
            if (chance <= INIT_CHANCE_ZOMBIE)
            {
                matrix[i][j].type = ZOMBIE;
                matrix[i][j].moveChance = MOVE_ZOMBIE;
            }
            else if (chance <= INIT_CHANCE_HUMAN)
            {
                matrix[i][j].type = HUMAN;
                matrix[i][j].moveChance = MOVE_HUMAN;
            }
            else
                matrix[i][j].type = EMPTY;
        }
    }
}

void clearMatrix(Entity **matrix)
{
}

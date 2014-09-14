#include <stdlib.h>

#include "entity.h"
#include "constants.h"

Entity **createMatrix(int size_x, int size_y)
{
    Entity **matrix = (Entity **) malloc(size_y * sizeof(Entity));

    int i, j;

    for (i = 0; i < size_y; i++)
    {
        matrix[i] = (Entity *) malloc(size_x * sizeof(Entity));
        for (j = 0; j < size_x; j++)
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
    int i, j;
    int choose = rand()%10;

    for (i = 1; i <= size_y; i++)
    {
        for (j = 1; j <= size_x; j++)
        {  if (choose > 5)
           {
            imatrix[i][j].type = HUMAN;
            matrix[i][j].moveChance = MOVE_HUMAN;
           }
           else if (choose <2)
           {
            imatrix[i][j].type = ZOMBIE;
            matrix[i][j].moveChance = MOVE_ZOMBIE;
            }
            else
                {imatrix[i][j].type = EMPTY;}
            
        }
    }
}

void clearMatrix(Entity **matrix)
{
}

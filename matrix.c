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
    srand48(8767134);

    int i, j;
    int choose = rand()%10;

    for (i = 1; i <= size_y; i++)
    {
        for (j = 1; j <= size_x; j++)
<<<<<<< HEAD
        {
            if(drand48() <= 0.10){
                matrix[i][j].type = ZOMBIE;
                matrix[i][j].moveChance = MOVE_ZOMBIE;
            }else{
                matrix[i][j].type = HUMAN;
                matrix[i][j].moveChance = MOVE_HUMAN;
            }
=======
        {  if (choose >2)
           {
            imatrix[i][j].type = HUMAN;
            matrix[i][j].moveChance = MOVE_HUMAN;
           }
           else 
           {
            imatrix[i][j].type = ZOMBIE;
            matrix[i][j].moveChance = MOVE_ZOMBIE;
            }
          
>>>>>>> FETCH_HEAD
            
        }
    }
}

void clearMatrix(Entity **matrix)
{
}

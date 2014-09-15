#include <stdio.h>

#include "utils.h"
#include "constants.h"
#include "entity.h"

void print_population(Entity **matrix, int step)
{
    int total_human = 0,
        total_zombie = 0,
        total_empty = 0;

    for (int i = 1; i <= SIZEY; i++)
    {
        for (int j = 1; j <= SIZEX; j++)
        {
            switch(matrix[i][j].type)
            {
            case HUMAN:
                total_human++;
                break;
            case ZOMBIE:
                total_zombie++;
                break;
            default:
                total_empty++;
            }
        }
    }

    printf("step:%d\thuman:%d\tzombie:%d\tempty:%d\n", step, total_human, total_zombie, total_empty);
}

void print_matrix(Entity **matrix, int t)
{
    for (int i = 1; i <= SIZEY; i++)
    {
        for (int j = 1; j <= SIZEX; j++)
        {
            char cell = 'e';

            switch(matrix[i][j].type)
            {
            case HUMAN:
                cell = 'h';
                break;
            case ZOMBIE:
                cell = 'z';
                break;
            default:
                ;
            }

            printf("%c\t", cell);
        }
        printf("\n");
    }
}

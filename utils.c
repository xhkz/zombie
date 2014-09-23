#include <stdio.h>

#include "utils.h"
#include "constants.h"
#include "entity.h"

void print_population(Entity **matrix, int step)
{
    int total_male = 0,
        total_female = 0,
        total_baby = 0,
        total_young = 0,
        total_adult = 0,
        total_elder = 0,
        total_zombie = 0,
        total_empty = 0;

    for (int i = 1; i <= SIZEY; i++)
    {
        for (int j = 1; j <= SIZEX; j++)
        {
            switch(matrix[i][j].type)
            {
            case HUMAN:
                if (matrix[i][j].gender == MALE) total_male++;
                if (matrix[i][j].gender == FEMALE) total_female++;
                if (matrix[i][j].stage == BABY) total_baby++;
                if (matrix[i][j].stage == YOUNG) total_young++;
                if (matrix[i][j].stage == ADULT) total_adult++;
                if (matrix[i][j].stage == ELDER) total_elder++;
                break;
            case ZOMBIE:
                total_zombie++;
                break;
            default:
                total_empty++;
            }
        }
    }

    printf("%d,male:%d,female:%d,baby:%d,young:%d,adult:%d,elder:%d,zombie:%d,empty:%d\n",
           step, total_male, total_female, total_baby, total_young, total_adult, total_elder, total_zombie, total_empty);
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

            printf("%c|", cell);
        }
        printf("\n");
    }
}

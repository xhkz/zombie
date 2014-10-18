#include <stdio.h>
#ifdef _OPENMP
#include <omp.h>
#endif

#include "utils.h"
#include "constants.h"
#include "entity.h"

Counter counter;

void update_counter(Entity **matrix)
{
    unsigned long male = 0,
                  female = 0,
                  baby = 0,
                  young = 0,
                  adult = 0,
                  elder = 0,
                  zombie = 0;
#ifdef _OPENMP
    #pragma omp parallel for default(shared) reduction(+:male,female,baby,young,adult,elder,zombie)
#endif
    for (int i = 1; i <= SIZEY; i++)
    {
        for (int j = 1; j <= SIZEX; j++)
        {
            switch(matrix[i][j].type)
            {
            case HUMAN:
                if (matrix[i][j].gender == MALE) male++;
                if (matrix[i][j].gender == FEMALE) female++;
                if (matrix[i][j].stage == BABY) baby++;
                if (matrix[i][j].stage == YOUNG) young++;
                if (matrix[i][j].stage == ADULT) adult++;
                if (matrix[i][j].stage == ELDER) elder++;
                break;
            case ZOMBIE:
                zombie++;
                break;
            default:
                ;
            }
        }
    }

    counter.male = male;
    counter.female = female;
    counter.baby = baby;
    counter.young = young;
    counter.adult = adult;
    counter.elder = elder;
    counter.zombie = zombie;
}

void print_population(int step)
{
    printf("%d, male:%lu, female:%lu, baby:%lu, young:%lu, adult:%lu, elder:%lu, zombie:%lu, population:%lu\n",
           step, counter.male, counter.female, counter.baby, counter.young, counter.adult, counter.elder, counter.zombie,
           (counter.male + counter.female + counter.zombie));
}

void print_matrix(Entity **matrix, int t)
{
    for (int i = 1; i <= SIZEY; i++)
    {
        for (int j = 1; j <= SIZEX; j++)
        {
            char cell = ' ';

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

void print_header(void)
{
    printf("days,male,female,baby,young,adult,elder,zombie,population\n");
}

void print_csv(int step)
{
    printf("%d,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu\n",
           step, counter.male, counter.female, counter.baby, counter.young, counter.adult, counter.elder, counter.zombie,
           (counter.male + counter.female + counter.zombie));
}

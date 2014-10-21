#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "utils.h"
#include "constants.h"
#include "entity.h"

Counter counter;

void updateCounter(Entity **matrix)
{
    unsigned long male = 0,
                  female = 0,
                  baby = 0,
                  young = 0,
                  adult = 0,
                  elder = 0,
                  zombie = 0;

    #pragma omp parallel for default(shared) reduction(+:male,female,baby,young,adult,elder,zombie)
    for (int i = 0; i < SIZEX + 2; i++)
    {
        #pragma omp parallel for
        for (int j = 0; j < SIZEY + 2; j++)
        {

            switch(matrix[i][j].type)
            {
            case HUMAN:
                #pragma omp parallel
                {
                    if (matrix[i][j].gender == MALE) male++;
                    if (matrix[i][j].gender == FEMALE) female++;
                    if (matrix[i][j].stage == BABY) baby++;
                    if (matrix[i][j].stage == YOUNG) young++;
                    if (matrix[i][j].stage == ADULT) adult++;
                    if (matrix[i][j].stage == ELDER) elder++;
                }
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

void mergeCounter(Counter counterBuffer)
{
    counter.male += counterBuffer.male;
    counter.female += counterBuffer.female;
    counter.baby += counterBuffer.baby;
    counter.young += counterBuffer.young;
    counter.adult += counterBuffer.adult;
    counter.elder += counterBuffer.elder;
    counter.zombie += counterBuffer.zombie;
}

void printPopulation(int step)
{
    printf("%d, male:%lu, female:%lu, baby:%lu, young:%lu, adult:%lu, elder:%lu, zombie:%lu, population:%lu\n",
           step, counter.male, counter.female, counter.baby, counter.young, counter.adult, counter.elder, counter.zombie,
           (counter.male + counter.female + counter.zombie));
}

/* DEBUG USE */
void printMatrix(Entity **matrix, int tag)
{
    char * msg = (char *) malloc(((SIZEX + 2)*(SIZEY + 2) + SIZEY + 3)*sizeof(char));
    char * msg_end = msg;

    for (int y = 0; y < SIZEY + 2; y++)
    {
        for (int x = 0; x < SIZEX + 2; x++)
        {
            char cell = '.';

            switch(matrix[x][y].type)
            {
            case HUMAN:
                switch(matrix[x][y].gender)
                {
                case FEMALE:
                    cell = 'f';
                    break;
                case MALE:
                    cell = 'm';
                    break;
                default:
                    ;
                }
                break;
            case ZOMBIE:
                cell = 'z';
                break;
            default:
                ;
            }

            msg_end += sprintf(msg_end, "%c", cell);
        }
        msg_end += sprintf(msg_end, "\n");
    }

    char tagM = (tag == 0 ? 'N' : 'S');

    printf("--tag: %c--\n%s\n", tagM, msg);
    free(msg);
}

void printHeader(void)
{
    printf("days,male,female,baby,young,adult,elder,zombie,population\n");
}

void printCSV(int step)
{
    printf("%d,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu\n",
           step, counter.male, counter.female, counter.baby, counter.young, counter.adult, counter.elder, counter.zombie,
           (counter.male + counter.female + counter.zombie));
}

void lock(int i, bool *locks)
{
    for (bool locked = false; locked == false; /*NOP*/)
    {
        #pragma omp critical (LockRegion)
        {
            locked = !locks[i-1] && !locks[i] && !locks[i+1];
            if (locked)
            {
                locks[i-1] = true;
                locks[i] = true;
                locks[i+1] = true;
            }
        }
    }
}

void unlock(int i, bool *locks)
{
    #pragma omp critical (LockRegion)
    {
        locks[i-1] = false;
        locks[i] = false;
        locks[i+1] = false;
    }
}
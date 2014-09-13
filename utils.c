#include <stdio.h>

#include "utils.h"
#include "constants.h"
#include "entity.h"

void print_population(Entity **matrix, int t)
{
    int total = 0;

    for (int i = 1; i <= SIZEY; i++)
    {
        for (int j = 1; j <= SIZEX; j++)
        {
            if (matrix[i][j].type == HUMAN)
                total++;
        }
    }

    printf("%d\t%d\n", t, total);
}

void print_matrix(Entity **matrix, int t)
{
    int i, j;

    for (i = 1; i <= SIZEY; i++)
    {
        for (j = 1; j <= SIZEX; j++)
        {
            printf("%s\t", matrix[i][j].type == EMPTY ? "e" : matrix[i][j].type == HUMAN ? "h" : "z");
        }
        printf("\n");
    }
}

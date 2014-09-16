#include <stdlib.h>
#include <stdbool.h>

#include "entity.h"
#include "constants.h"

bool randomDeath(Entity * p, double val)
{
    return (p->type == HUMAN && val < DEATH_RATE_HUMAN) ||
           (p->type == ZOMBIE && val < DEATH_RATE_ZOMBIE);
}

void process(Entity **matrix_a, Entity **matrix_b, int i, int j)
{
    Entity *cell_a = &matrix_a[i][j];

    if (cell_a->type != EMPTY)
    {
        double move = drand48();
        double moveChance = cell_a->moveChance;
        Entity * cell_b = NULL;
        if (move < 1.0*moveChance && matrix_a[i-1][j].type == EMPTY && matrix_b[i-1][j].type == EMPTY)
        {
            cell_b = &matrix_b[i-1][j];
        }
        else if (move < 2.0*moveChance && matrix_a[i+1][j].type == EMPTY && matrix_b[i+1][j].type == EMPTY)
        {
            cell_b = &matrix_b[i+1][j];
        }
        else if (move < 3.0*moveChance && matrix_a[i][j-1].type == EMPTY && matrix_b[i][j-1].type == EMPTY)
        {
            cell_b = &matrix_b[i][j-1];
        }
        else if (move < 4.0*moveChance && matrix_a[i][j+1].type == EMPTY && matrix_b[i][j+1].type == EMPTY)
        {
            cell_b = &matrix_b[i][j+1];
        }
        else
        {
            cell_b = &matrix_b[i][j];
        }

        if (!randomDeath(cell_a, move)) copyEntity(cell_a, cell_b);
        clearEntity(cell_a);
    }
}

void moveBackBorder(Entity **matrix)
{
    for (int i = 1; i <= SIZEX; i++)
    {
        moveEntity(&matrix[i][0], &matrix[i][1]);
        moveEntity(&matrix[i][SIZEY+1], &matrix[i][SIZEY]);
    }

    for (int i = 1; i <= SIZEY; i++)
    {
        moveEntity(&matrix[0][i], &matrix[1][i]);
        moveEntity(&matrix[SIZEX+1][i], &matrix[SIZEX][i]);
    }
}

void moveEntity(Entity * src, Entity * dest)
{
    if (src->type != EMPTY && dest->type == EMPTY)
    {
        copyEntity(src, dest);
        clearEntity(src);
    }
}

void clearEntity(Entity * p)
{
    p->type = EMPTY;
    p->gender = INV;
    p->stage = NIL;
    p->steps = 0;
    p->moveChance = 0.0;
    p->status = NONE;
}

void copyEntity(Entity * src, Entity * dest)
{
    dest->type = src->type;
    dest->gender = src->gender;
    dest->stage = src->stage;
    dest->moveChance = src->moveChance;
    dest->status = src->status;
    dest->steps = src->steps;
}

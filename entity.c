#include <stdlib.h>

#include "entity.h"

void moveEntity(Entity **matrix_a, Entity **matrix_b, int i, int j)
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

        copyEntity(cell_a, cell_b);
        clearEntity(cell_a);
    }
}

void moveBackBorder(Entity **matrix)
{

}

void clearEntity(Entity * p)
{
    p->type = EMPTY;
    p->age = 0;
    p->steps = 0;
    p->moveChance = 0.0;
    p->status = NONE;
}

void copyEntity(Entity * source, Entity * target)
{
    target->type = source->type;
    target->age = source->age;
    target->steps = source->steps;
    target->moveChance = source->moveChance;
    target->status = source->status;
}

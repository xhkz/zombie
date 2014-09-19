#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "entity.h"
#include "constants.h"


bool randomDeath(Entity * p, double val)
{
    return (p->type == HUMAN && val < DEATH_RATE_HUMAN) ||
           (p->type == ZOMBIE && val < DEATH_RATE_ZOMBIE);
}

void randomBirth(Entity * p, Entity **matrix_a, Entity **matrix_b, int i, int j)
{
    if (p->type == HUMAN && p->stage == ADULT && p->status == HEALTHY)
    {
        //check neighbors, return
        if (i > 2 && pairBirth(p, &matrix_a[i-1][j], &matrix_b[i-2][j])) return;
        if (i < SIZEX && pairBirth(p, &matrix_a[i+1][j], &matrix_b[i+2][j])) return;
        if (j > 2 && pairBirth(p, &matrix_a[i][j-1], &matrix_b[i][j-2])) return;
        if (j < SIZEY && pairBirth(p, &matrix_a[i][j+1], &matrix_b[i][j+2])) return;
    }
}

bool pairBirth(Entity * p, Entity * neighbor, Entity * child)
{
    if (neighbor->type == HUMAN && neighbor->stage == ADULT && neighbor->gender != p->gender
            && neighbor->status == HEALTHY && child->type == EMPTY && drand48() < BIRTH_RATE_HUMAN)
    {
        createHuman(child, drand48());
        return true;
    }
    return false;
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

void createHuman(Entity * p, double rnd)
{
    p->type = HUMAN;
    p->gender = rnd > INIT_GENDER_RATE ? MALE : FEMALE;
    if (rnd < INIT_BABY_RATE)
    {
        p->stage = BABY;
        p->moveChance = MOVE_HUMAN_BABY;
    }
    else if (rnd < INIT_YOUNG_RATE)
    {
        p->stage = YOUNG;
        p->moveChance = MOVE_HUMAN_YOUNG;
    }
    else if (rnd < INIT_ADULT_RATE)
    {
        p->stage = ADULT;
        p->moveChance = MOVE_HUMAN_ADULT;
    }
    else
    {
        p->stage = ELDER;
        p->moveChance = MOVE_HUMAN_ELDER;
    }

    p->status = HEALTHY;
}

void createZombie(Entity * p)
{
    p->type = ZOMBIE;
    p->gender = INV;
    p->stage = NIL;
    p->moveChance = MOVE_ZOMBIE;
    p->status = NONE;
}

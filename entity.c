#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "entity.h"
#include "constants.h"


bool randomDeath(Entity * p, double rnd)
{
    return (p->type == HUMAN && rnd < DEATH_RATE_HUMAN) ||
           (p->type == ZOMBIE && rnd < DEATH_RATE_ZOMBIE);
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
        createHuman(child, 0.0, BABY);
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
    p->age = 0;
    p->stage = NIL;
    p->steps = 0;
    p->moveChance = 0.0;
    p->status = NONE;
}

void copyEntity(Entity * src, Entity * dest)
{
    dest->type = src->type;
    dest->gender = src->gender;
    dest->age = src->age;
    dest->stage = src->stage;
    dest->moveChance = src->moveChance;
    dest->status = src->status;
    dest->steps = src->steps;
}

void createHuman(Entity * p, double rnd, Stage s)
{
    p->type = HUMAN;
    p->gender = rnd > INIT_GENDER_RATE ? MALE : FEMALE;
    if (rnd < INIT_BABY_RATE || s == BABY)
    {
        p->age = (int)(drand48() * AGE_BABY_MAX);
        p->stage = BABY;
        p->moveChance = MOVE_HUMAN_BABY;
    }
    else if (rnd < INIT_YOUNG_RATE || s == YOUNG)
    {
        p->age = (int)(AGE_BABY_MAX + drand48() * (AGE_YOUNG_MAX - AGE_BABY_MAX));
        p->stage = YOUNG;
        p->moveChance = MOVE_HUMAN_YOUNG;
    }
    else if (rnd < INIT_ADULT_RATE || s == ADULT)
    {
        p->age = (int)(AGE_YOUNG_MAX + drand48() * (AGE_ADULT_MAX - AGE_YOUNG_MAX));
        p->stage = ADULT;
        p->moveChance = MOVE_HUMAN_ADULT;
    }
    else
    {
        p->age = (int)(AGE_ADULT_MAX + drand48() * (AGE_ELDER_MAX - AGE_ADULT_MAX));
        p->stage = ELDER;
        p->moveChance = MOVE_HUMAN_ELDER;
    }

    p->status = HEALTHY;
}

void createZombie(Entity * p)
{
    p->type = ZOMBIE;
    p->moveChance = MOVE_ZOMBIE;
}

void growup(Entity * entity)
{
    if (entity->type == HUMAN)
    {
        if (entity->steps % YEAR == 0) entity->age++;

        switch(entity->stage)
        {
        case BABY:
            if (entity->age >= AGE_BABY_MAX) entity->stage = YOUNG;
            break;
        case YOUNG:
            if (entity->age >= AGE_YOUNG_MAX) entity->stage = ADULT;
            break;
        case ADULT:
            if (entity->age >= AGE_ADULT_MAX) entity->stage = ELDER;
            break;
        case ELDER:
            if (entity->age >= AGE_ELDER_MAX) clearEntity(entity);
            break;
        default:
            ;
        }
    }
}

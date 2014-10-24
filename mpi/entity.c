#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "entity.h"
#include "constants.h"
#include "random.h"

bool randomDeath(Entity * p)
{
    double rnd = drandom();

    if (!benchmark && p->type == ZOMBIE && counter.zombie <= ZOMBIE_DIE_HARD)
        return false;

    return (p->type == HUMAN && rnd < DEATH_RATE_HUMAN) ||
           (p->type == ZOMBIE && rnd < DEATH_RATE_ZOMBIE);
}

bool pairBirth(Entity * p, Entity * neighbor, Entity * child)
{
    if (neighbor->type == HUMAN && neighbor->stage == ADULT && neighbor->gender != p->gender
            && neighbor->status == HEALTHY && child->type == EMPTY && drandom() < BIRTH_RATE_PAIR)
    {
        createHuman(child, BABY);
        child->age = 0;
        child->steps = 0;
        return true;
    }
    return false;
}

void randomBirth(Entity * p, Entity **matrix_a, Entity **matrix_b, int i, int j)
{
    if (p->type == HUMAN && p->stage == ADULT && p->status == HEALTHY)
    {
        //check neighbors, return
        if (pairBirth(p, &matrix_a[i-1][j], &matrix_b[i-1][j-1])) return;
        if (pairBirth(p, &matrix_a[i+1][j], &matrix_b[i+1][j+1])) return;
        if (pairBirth(p, &matrix_a[i][j-1], &matrix_b[i+1][j-1])) return;
        if (pairBirth(p, &matrix_a[i][j+1], &matrix_b[i-1][j+1])) return;
    }
}

bool pairInfection(Entity * p, Entity * neighbor)
{
    double rate = INFECTION_RATE;

    if (!benchmark && counter.zombie < ZOMBIE_DIE_HARD) rate = 1.0;

    if (p->type == HUMAN && neighbor->type == ZOMBIE && drandom() < rate)
    {
        createZombie(p);
        return true;
    }

    if (p->type == ZOMBIE && neighbor->type == HUMAN && drandom() < rate)
    {
        createZombie(neighbor);
        return true;
    }

    return false;
}

void randomInfection(Entity * p, Entity **matrix_a, int i, int j)
{
    //human who are infected will become zombies within one day
    pairInfection(p, &matrix_a[i-1][j]);
    pairInfection(p, &matrix_a[i+1][j]);
    pairInfection(p, &matrix_a[i][j-1]);
    pairInfection(p, &matrix_a[i][j+1]);
}

void randomWalk(Entity * cell_a, Entity ** cell_b, Entity **matrix_a, Entity **matrix_b, int i, int j)
{
    if (cell_a->type != EMPTY) {
        double move = drandom();
        double moveChance = cell_a->moveChance;

        if (move < 1.025*moveChance && matrix_a[i-1][j].type == EMPTY && matrix_b[i-1][j].type == EMPTY)
        {
            * cell_b = &matrix_b[i-1][j];
        }
        else if (move < 2.0*moveChance && matrix_a[i+1][j].type == EMPTY && matrix_b[i+1][j].type == EMPTY)
        {
            * cell_b = &matrix_b[i+1][j];
        }
        else if (move < 3.0*moveChance && matrix_a[i][j-1].type == EMPTY && matrix_b[i][j-1].type == EMPTY)
        {
            * cell_b = &matrix_b[i][j-1];
        }
        else if (move < 4.0*moveChance && matrix_a[i][j+1].type == EMPTY && matrix_b[i][j+1].type == EMPTY)
        {
            * cell_b = &matrix_b[i][j+1];
        }
        else
        {
            * cell_b = &matrix_b[i][j];
        }
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

void createHuman(Entity * p, Stage s)
{
    double stageRandom = drandom();

    p->type = HUMAN;
    p->gender = drandom() > INIT_GENDER_RATE ? MALE : FEMALE;

    if (stageRandom < INIT_BABY_RATE || s == BABY)
    {
        p->age = irandom(AGE_BABY_MAX);
        p->stage = BABY;
        p->moveChance = MOVE_HUMAN_BABY;
    }
    else if (stageRandom < INIT_YOUNG_RATE || s == YOUNG)
    {
        p->age = AGE_BABY_MAX + irandom(AGE_YOUNG_MAX - AGE_BABY_MAX);
        p->stage = YOUNG;
        p->moveChance = MOVE_HUMAN_YOUNG;
    }
    else if (stageRandom < INIT_ADULT_RATE || s == ADULT)
    {
        p->age = AGE_YOUNG_MAX + irandom(AGE_ADULT_MAX - AGE_YOUNG_MAX);
        p->stage = ADULT;
        p->moveChance = MOVE_HUMAN_ADULT;
    }
    else
    {
        p->age = AGE_ADULT_MAX + irandom(AGE_ELDER_MAX - AGE_ADULT_MAX);
        p->stage = ELDER;
        p->moveChance = MOVE_HUMAN_ELDER;
    }

    p->status = HEALTHY;
    p->steps = irandom(YEAR);
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
        if (entity->steps == YEAR)
        {
            entity->age++;
            entity->steps = 0;
        }

        switch(entity->stage)
        {
        case BABY:
            if (entity->age > AGE_BABY_MAX) entity->stage = YOUNG;
            break;
        case YOUNG:
            if (entity->age > AGE_YOUNG_MAX) entity->stage = ADULT;
            break;
        case ADULT:
            if (entity->age > AGE_ADULT_MAX) entity->stage = ELDER;
            break;
        case ELDER:
            if (entity->age > AGE_ELDER_MAX) clearEntity(entity);
            break;
        default:
            ;
        }
    }
}

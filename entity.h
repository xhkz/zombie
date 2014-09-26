#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <stdbool.h>

typedef enum {
    MALE, FEMALE, INV
} Gender;

typedef enum {
    BABY, YOUNG, ADULT, ELDER, NIL
} Stage;

typedef enum {
    HUMAN, ZOMBIE, EMPTY
} EntityType;

typedef enum {
    HEALTHY, INFECTED, NONE
} Status;

typedef struct {
    EntityType type;
    Gender gender;
    int age;
    Stage stage;
    double moveChance;
    Status status;
    unsigned long long int steps;
} Entity;

bool randomDeath(Entity * p);
void randomBirth(Entity * p, Entity **matrix_a, Entity **matrix_b, int i, int j);
bool pairBirth(Entity * p, Entity * neighbor, Entity * child);
void moveEntity(Entity * src, Entity * dest);
void clearEntity(Entity * p);
void copyEntity(Entity * source, Entity * target);
void createHuman(Entity * entity, Stage s);
void createZombie(Entity * entity);
void growup(Entity * entity);
bool pairInfection(Entity * p, Entity * neighbor);
void randomWalk(Entity * cell_a, Entity ** cell_b, Entity **matrix_a, Entity **matrix_b, int i, int j);
void randomInfection(Entity * p, Entity **matrix_a, Entity **matrix_b, int i, int j);

#endif // ENTITY_H_INCLUDED

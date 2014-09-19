#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <stdbool.h>

typedef enum {
    MALE, FEMALE, INV
} Gender;

typedef enum {
    YOUNG, ADULT, ELDER, NIL
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
    Stage stage;
    double moveChance;
    Status status;
    unsigned long long int steps;
} Entity;


void process(Entity **matrix_a, Entity **matrix_b, int i, int j);
bool randomDeath(Entity * p, double val);
void randomBirth(Entity * p, Entity **matrix_a, Entity **matrix_b, int i, int j);
bool pairBirth(Entity * p, Entity * neighbor, Entity * child);
void moveBackBorder(Entity **matrix);
void moveEntity(Entity * src, Entity * dest);
void clearEntity(Entity * p);
void copyEntity(Entity * source, Entity * target);
void createHuman(Entity * entity, double rnd);
void createZombie(Entity * entity);

#endif // ENTITY_H_INCLUDED

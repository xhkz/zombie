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

bool randomDeath(Entity * p, double val);
void process(Entity **matrix_a, Entity **matrix_b, int i, int j);
void moveBackBorder(Entity **matrix);
void moveEntity(Entity * src, Entity * dest);
void clearEntity(Entity * p);
void copyEntity(Entity * source, Entity * target);

#endif // ENTITY_H_INCLUDED

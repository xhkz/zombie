#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

typedef enum {
    MALE, FEMALE
} Gender;

typedef enum {
    HUMAN, ZOMBIE, EMPTY
} EntityType;

typedef enum {
    HEALTHY, INFECTED,
} Status;

typedef struct {
    EntityType type;
    int age;
    unsigned long long int steps;
    double moveChance;
    Status status;
} Entity;

#endif // ENTITY_H_INCLUDED

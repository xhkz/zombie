#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

typedef enum {
    MALE, FEMALE
} Gender;

typedef enum {
    YOUNG, ADULT, ELDER
} Stage;

typedef enum {
    HUMAN, ZOMBIE, EMPTY
} EntityType;

typedef enum {
    HEALTHY, INFECTED, NONE
} Status;

typedef struct {
    EntityType type;
    Stage stage;
    unsigned long long int steps;
    double moveChance;
    Status status;
} Entity;

void moveEntity(Entity **matrix_a, Entity **matrix_b, int i, int j);
void moveBackBorder(Entity **matrix);
void clearEntity(Entity * p);
void copyEntity(Entity * source, Entity * target);

#endif // ENTITY_H_INCLUDED

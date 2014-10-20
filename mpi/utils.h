#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "constants.h"
#include "entity.h"

void updateCounter(Entity **matrix_a, Entity **matrix_b);
void mergeCounter(Counter counterBuffer);
void printPopulation(int t);
void printMatrix(Entity **matrix, int tag);
void printHeader(void);
void printCSV(int step);
void lock(int i, bool *locks);
void unlock(int i, bool *locks);

#endif // UTILS_H_INCLUDED

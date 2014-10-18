#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "constants.h"
#include "entity.h"

void updateCounter(Entity **matrix);
void printPopulation(int t);
void printMatrix(Entity **matrix, int t);
void printHeader(void);
void printCSV(int step);

#endif // UTILS_H_INCLUDED

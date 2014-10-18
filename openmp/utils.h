#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "constants.h"
#include "entity.h"

void update_counter(Entity **matrix);
void print_population(int t);
void print_matrix(Entity **matrix, int t);
void print_header(void);
void print_csv(int step);

#endif // UTILS_H_INCLUDED

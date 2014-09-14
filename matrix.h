#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include "entity.h"

Entity **createMatrix(int size_x, int size_y);

void initMatrix(Entity **matrix, int size_x, int size_y);

void clearMatrix(Entity **matrix);

int rand(void);

#endif // MATRIX_H_INCLUDED

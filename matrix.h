#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include "entity.h"

Entity **createMatrix(int size_x, int size_y);
int randomPos(int limit);
void initMatrix(Entity **matrix, int size_x, int size_y);
void clearMatrix(Entity **matrix);

#endif // MATRIX_H_INCLUDED

#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

void initRandom(unsigned long seed, int rank);
double drandom(void);
int irandom(int limit);

#endif // RANDOM_H_INCLUDED


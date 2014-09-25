#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

static void init_twister(void);
static void sgenrand(unsigned long int rng_num, unsigned long seed);
static unsigned long genrand(unsigned long int rng_num);
void initRandom();
double drandom(void);
int irandom(int limit);

#endif // RANDOM_H_INCLUDED


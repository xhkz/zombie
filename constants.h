#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

#define STEPS   10
#define SIZEX   10
#define SIZEY   10

#define MAX_AGE 150

#define INIT_CHANCE_HUMAN   0.6
#define INIT_CHANCE_ZOMBIE  0.1

// move chance per day each direction
#define MOVE_HUMAN_YOUNG   0.1
#define MOVE_HUMAN_ADULT 0.1
#define MOVE_HUMAN_ELDER 0.1
#define MOVE_ZOMBIE  0.2

#define BIRTH_RATE_HUMAN  0.001
#define DEATH_RATE_HUMAN  0.001

#define DEATH_RATE_ZOMBIE 0.01
#define INFECT_RATE 0.01
#define INIT_ZOMBIE_NUM 2
#define INIT_HUMAN_RATE 0.1

#endif // CONSTANTS_H_INCLUDED

#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

#define STEPS   3650
#define SIZEX   500
#define SIZEY   500

#define INIT_HUMAN_NUM      6549
#define INIT_ZOMBIE_NUM     2
#define INIT_CHANCE_HUMAN   INIT_HUMAN_NUM/(SIZEX*SIZEY)
#define INIT_CHANCE_ZOMBIE  INIT_ZOMBIE_NUM/(SIZEX*SIZEY)

#define INIT_GENDER_RATE    0.5

// age: young(0-0.2) adult(0.2-0.8) elderly(0.8-1)
#define INIT_BABY_RATE      0.05 
#define INIT_YOUNG_RATE     0.2
#define INIT_ADULT_RATE     0.8

// move chance per day each direction
#define MOVE_HUMAN_BABY     0.025
#define MOVE_HUMAN_YOUNG    0.125
#define MOVE_HUMAN_ADULT    0.1
#define MOVE_HUMAN_ELDER    0.05
#define MOVE_ZOMBIE         0.2

#define BIRTH_RATE_HUMAN    0.0138
#define DEATH_RATE_HUMAN    0.00756
#define DEATH_RATE_ZOMBIE   0.02

#define INFECTION_RATE      0.05

#endif // CONSTANTS_H_INCLUDED

/*
    Demographics_of_Australia                  Model
    Area: 9,008,500 km2                        500*500 = 250000 km2
    Population: 23,599,700                     23599700*250000/9008500 (/100) = 6549
    Population density: 2.91 /km2              2.91 / 100
    Birth rate: 13.8 /1000
    Death rate: 7.56 /1000
    Age structure:
        0–14 years – 18.2%
        15–65 years – 67.5%
            15-24 years – 13.5%
            25-54 years – 42.2%
            55–64 years – 11.8%
        65 years and over – 14.4%

*/

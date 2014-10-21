#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#include <curses.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "constants.h"
#include "entity.h"
#include "utils.h"
#include "matrix.h"
#include "random.h"

int interval = 100;

#ifdef _OPENMP
void lock(int i, bool *locks)
{
    for (bool locked = false; locked == false; /*NOP*/)
    {
        #pragma omp critical (LockRegion)
        {
            locked = !locks[i-1] && !locks[i] && !locks[i+1];
            if (locked)
            {
                locks[i-1] = true;
                locks[i] = true;
                locks[i+1] = true;
            }
        }
    }
}

void unlock(int i, bool *locks)
{
    #pragma omp critical (LockRegion)
    {
        locks[i-1] = false;
        locks[i] = false;
        locks[i+1] = false;
    }
}
#endif

void init_curses(void)
{
    initscr();
    start_color();
    init_pair(0, COLOR_WHITE,  COLOR_BLACK);
    init_pair(1, COLOR_RED,    COLOR_BLACK);
    init_pair(2, COLOR_CYAN,   COLOR_BLACK);
    init_pair(3, COLOR_GREEN,  COLOR_BLACK);
    init_pair(4, COLOR_BLUE,   COLOR_BLACK);
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);

    atexit((void (*)()) endwin);
}

void end_curses(void)
{
    getchar();
}

void curses_draw(Entity **matrix, int step)
{
    int i, j;
    for (j = 0; j < SIZEY + 2; j++)
    {
        move(j, 0);
        for (i = 0; i < SIZEX + 2; i++)
        {
            char ch = '.';
            int color = 0;
            
            switch(matrix[i][j].type)
            {
            case HUMAN:
                if (matrix[i][j].gender == MALE) ch = 'm';
                if (matrix[i][j].gender == FEMALE) ch = 'f';
                if (matrix[i][j].stage == BABY) color = 2;
                if (matrix[i][j].stage == YOUNG) color = 3;
                if (matrix[i][j].stage == ADULT) color = 4;
                if (matrix[i][j].stage == ELDER) color = 5;
                break;
            case ZOMBIE:
                ch = 'z';
                color = 1;
                break;
            default:
                ;
            }

            attron(COLOR_PAIR(color));
            addch(ch);
            attroff(COLOR_PAIR(color));
        }
    }
    
    mvprintw(j, 0, "Step: %d, Male: %lu, Female: %lu, Zombie: %3lu",
           step, counter.male, counter.female, counter.zombie);           
    mvprintw(j + 1, 0, "CYAN: baby, GREEN: young, BLUE: adult, YELLOW: elderly");
    
    refresh();
    
    usleep(interval);
}

int main(int argc, char **argv)
{
    if (argc > 1) interval = atoi(argv[1]);

    initRandom(0);

    bool *locks = (bool *)malloc((SIZEX + 2) * sizeof(bool));

    for (int i = 0; i < SIZEX + 2; i++)
        locks[i] = false;

    Entity **matrix_a = createMatrix(SIZEX + 2, SIZEY + 2);
    Entity **matrix_b = createMatrix(SIZEX + 2, SIZEY + 2);

    initMatrix(matrix_a, SIZEX, SIZEY);
    init_curses();

    update_counter(matrix_a);
    for (int n = 0; n < STEPS; n++)
    {
#ifdef _OPENMP
        #pragma omp parallel for default(none) shared(matrix_a, matrix_b, n, locks, counter) schedule(static, SIZEX/omp_get_max_threads())
#endif
        for (int i = 1; i <= SIZEX; i++)
        {
#ifdef _OPENMP
            lock(i, locks);
#endif
            #pragma omp parallel for
            for (int j = 1; j <= SIZEY; j++)
            {
                process(matrix_a, matrix_b, i, j);
            }
#ifdef _OPENMP
            unlock(i, locks);
#endif
        }

        transferInBorder(matrix_a, matrix_b);
        moveBackInBorder(matrix_b);

        //swap matrix
        Entity **matrix_t = matrix_a;
        matrix_a = matrix_b;
        matrix_b = matrix_t;
        update_counter(matrix_a);
        curses_draw(matrix_a, n + 1);
    }

    destroyMatrix(matrix_a);
    destroyMatrix(matrix_b);

    end_curses();

    return 0;
}

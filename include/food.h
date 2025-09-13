#ifndef FOOD_H
#define FOOD_H

#include <ncurses.h>

struct Food {
    int line;
    int col;
};

struct Food create_food_particle(void);
void draw_food_particle(WINDOW *window, int line, int col);

#endif
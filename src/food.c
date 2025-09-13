#include <food.h>
#include <game.h>
#include <config.h>

#include <stdlib.h>

struct Food create_food_particle(void) {
    int line = rand() % PLAYABLE_ZONE_LINES;
    int col = rand() % PLAYABLE_ZONE_COLS;

    struct Food food_particle;
    food_particle.line = line;
    food_particle.col = col;
    return food_particle;
}

void draw_food_particle(WINDOW *window, int line, int col) {
    wmove(window, line, col);
    waddch(window, '@');
    wrefresh(window);
}
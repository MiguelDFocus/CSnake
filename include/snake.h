#ifndef SNAKE_H
#define SNAKE_H

#include <game.h>
#include <food.h>

struct Snake {
    int line;
    int col;
    struct Snake *next_snake;
};

void draw_snake(struct Snake *snake, WINDOW *window);
struct Snake *create_snake(int line, int col);
struct Snake *get_tail(struct Snake *snake);
void increase_snake_length(struct Snake *snake, char direction);
void move_snake(struct GameState *game);
bool window_border_touched(int line, int col);
bool tail_touched(struct Snake *snake, int line, int col);
void destroy_snake(struct Snake *snake);
bool detect_food_collision(struct Snake *snake, struct Food food);

#endif
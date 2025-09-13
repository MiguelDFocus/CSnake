#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include <food.h>

struct Snake;

struct GameState {
    int start_line;
    int start_col;
    int line;
    int col;
    int score;
    char input;
    char direction;

    struct Snake *snake;
    struct Food food;

    WINDOW *main_window;
    WINDOW *play_window;
    WINDOW *food_window;
    WINDOW *score_window;
};

void draw_playing_zone(WINDOW *window);
void draw_score(WINDOW *window, int score);
void draw_end_game_message(WINDOW *window);
bool is_input_correct(char input);
bool is_direction_opposite(char input, char direction);

#endif
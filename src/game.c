#include <config.h>
#include <game.h>

#include <unistd.h>

void draw_playing_zone(WINDOW *window) {
    box(window, 0, 0);
    wrefresh(window);
}

void draw_score(WINDOW *window, int score) {
    werase(window);
    wmove(window, SCORE_ZONE_LINE_START, SCORE_ZONE_COL_START);
    wprintw(window, "Score: %i", score);
    wrefresh(window);
}

void draw_end_game_message(WINDOW *window) {
    int base_string_length = 10;
    
    werase(window);
    wmove(window, START_LINE, START_COL - (base_string_length / 2));
    wprintw(window, "Game over!");
    wrefresh(window);
    usleep(1500000);
}

bool is_input_correct(char input) {
    for (int i = 0; i < (sizeof(DIRECTIONS) / sizeof(DIRECTIONS[0])); i++) {
        if (input == DIRECTIONS[i]) {
            return true;
        }
    }
    return false;
}

bool is_direction_opposite(char input, char direction) {
    switch (direction) {
        case 'w':
            if (input == 's') {
                return true;
            }
            break;
        case 's':
            if (input == 'w') {
                return true;
            }
            break;
        case 'a':
            if (input == 'd') {
                return true;
            }
            break;
        case 'd':
            if (input == 'a') {
                return true;
            }
            break;
    }
    return false;
}
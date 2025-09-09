/*
Steps to create the game:
    - Start ncurses - Done
    - Have a endless loop with ncurses open - Done
    - Draw a white square to the screen - Done
    - Make the white square move 1 block by user input - Done
    - Make the square move forward automatically and only change direction if input is pressed -- Done
    - Add collision to the borders of the screen -- Done
    - Add death and break out of loop if collission is detected -- Done
    - Add death when touching your own tail
    - Add food, make it spawn on random points of the screen
    - Add collision to the food, make it disappear on collision with snake
    - Increase length of snake when food is consumed
    - Keep track of the score
*/


#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>

WINDOW *draw_playzone(void);
void move_snake(WINDOW *play_window, char direction, int *x, int *y);
bool window_border_touched(int x, int y);
bool is_input_correct(char input);
bool is_direction_opposite(char input, char direction);
void show_end_game_message(WINDOW *play_window, int start_x_position, int start_y_position, int score);

const int MAIN_WINDOW_COLS = 120, MAIN_WINDOW_ROWS = 30;
const int PLAYABLE_ZONE_COLS = MAIN_WINDOW_COLS - 1, PLAYABLE_ZONE_ROWS = MAIN_WINDOW_ROWS - 1;
const int PLAYABLE_ZONE_X_START = 1, PLAYABLE_ZONE_Y_START = 1;
const char DIRECTIONS [4] = {'w', 'a', 's', 'd'};

int main(void) {
	initscr(); // Start curses
    noecho(); // Avoid user inputs to appear on screen

    int score = 0;
    char input; // Initialise input to be populated on loop
    char direction = 'd'; // Snake will start moving to the right when game starts
    int start_x_position = PLAYABLE_ZONE_COLS / 2, start_y_position = PLAYABLE_ZONE_ROWS / 2; 
    int x = start_x_position, y = start_y_position; // Get the middle of the playable window
    
    draw_playzone(); // Draw a square to act as play zone
    WINDOW *play_window = newwin(PLAYABLE_ZONE_ROWS, PLAYABLE_ZONE_COLS, PLAYABLE_ZONE_X_START, PLAYABLE_ZONE_X_START);
    nodelay(play_window, true); // Make listening for input non blocking
    
    while ((input = wgetch(play_window)) != 'q') {
        // If a key was pressed, assign direction
        if (input != ERR && is_input_correct(input)) {
            if (!is_direction_opposite(input, direction)) {
                direction = input;
            }
        }
        move_snake(play_window, direction, &x, &y);

        // Finish game if border is touched
        if (window_border_touched(x, y)) {
            usleep(500000);
            werase(play_window);
            wrefresh(play_window);
            show_end_game_message(play_window, start_x_position, start_y_position, score);
            break;
        }
        usleep(150000);
    }

	endwin();
	return 0;
}

WINDOW *draw_playzone(void) {
    WINDOW *win = newwin(MAIN_WINDOW_ROWS, MAIN_WINDOW_COLS, MAIN_WINDOW_ROWS, MAIN_WINDOW_COLS);
    for (int i = 0; i < MAIN_WINDOW_COLS; i++) {
        move(0, i);
        addch('*');
        move(MAIN_WINDOW_ROWS, i);
        addch('*');
    }
    for (int i = 0; i < MAIN_WINDOW_ROWS; i++) {
        move(i, 0);
        addch('*');
        move(i, MAIN_WINDOW_COLS);
        addch('*');
    }
    return win;
}

void move_snake(WINDOW *play_window, char direction, int *x, int *y) {
    switch (direction) {
        case 'w':
            (*y)--;
            break;
        case 's':
            (*y)++;
            break;
        case 'a':
            (*x)--;
            break;
        case 'd':
            (*x)++;
            break;
        default:
            break;
    }
    werase(play_window);
    wmove(play_window, *y, *x);
    waddch(play_window, '#');
    refresh();
}

bool window_border_touched(int x, int y) {
    if ((x <= 1) | (x >= PLAYABLE_ZONE_COLS) | (y <= 1) | (y >= PLAYABLE_ZONE_ROWS)) {
        return true;
    }
    return false;
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

void show_end_game_message(WINDOW *play_window, int start_x_position, int start_y_position, int score) {
    // Get the length of the base string, easier to do this way because string formatting is complicated
    int base_string_length = 18; // "Game over! Score: <score>"
    int score_length = 1;
    // Ugly but fast way to check digits of an int, just check for 10, 100 and 1000
    if (score < 10) {
        score_length = 2;
    } else if (score < 100) {
        score_length = 3;
    } else if (score < 1000) {
        score_length = 4;
    }
    base_string_length += score_length;

    move(start_y_position, start_x_position - (base_string_length / 2));
    printw("Game over! Score: %i", score);
    refresh();
    usleep(1500000);
}
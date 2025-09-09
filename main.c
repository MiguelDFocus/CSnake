/*
Steps to create the game:
    - Start ncurses - Done
    - Have a endless loop with ncurses open - Done
    - Draw a white square to the screen - Done
    - Make the white square move 1 block by user input - Done
    - Make the square move forward automatically and only change direction if input is pressed
    - Add collision to the borders of the screen
    - Add death and break out of loop if collission is detected
    - Add food, make it spawn on random points of the screen
    - Add collision to the food, make it disappear on collision with snake
    - Increase length of snake when food is consumed
    - Keep track of the score
*/


#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>

WINDOW *draw_playzone(void);
bool is_input_correct(char input);
bool is_direction_opposite(char input, char direction);

int PLAYZONE_X = 120, PLAYZONE_Y = 30;
char DIRECTIONS [4] = {'w', 'a', 's', 'd'};

int main(void) {
	initscr(); // Start curses
    noecho(); // Avoid user inputs to appear on screen
    char input;
    char direction = 'd';
    int y = PLAYZONE_Y / 2, x = PLAYZONE_X / 2;
    
    WINDOW *main_window = draw_playzone();
    WINDOW *play_window = newwin(PLAYZONE_Y - 1, PLAYZONE_X - 1, 1, 1);
    nodelay(play_window, true); // Make listening for input non blocking
    
    while ((input = wgetch(play_window)) != 'q') {
        // If a key was pressed, assign direction
        if (input != ERR && is_input_correct(input)) {
            if (!is_direction_opposite(input, direction)) {
                direction = input;
            }
        }
        switch (direction) {
            case 'w':
                y--;
                break;
            case 's':
                y++;
                break;
            case 'a':
                x--;
                break;
            case 'd':
                x++;
                break;
            default:
                break;
        }
        werase(play_window);
        wmove(play_window, y, x);
        waddch(play_window, '#');
        refresh();
        usleep(150000);
    }

	endwin();
	return 0;
}

WINDOW *draw_playzone(void) {
    WINDOW *win = newwin(PLAYZONE_Y, PLAYZONE_X, PLAYZONE_Y, PLAYZONE_X);
    for (int i = 0; i < PLAYZONE_X; i++) {
        move(0, i);
        addch('*');
        move(PLAYZONE_Y, i);
        addch('*');
    }
    for (int i = 0; i < PLAYZONE_Y; i++) {
        move(i, 0);
        addch('*');
        move(i, PLAYZONE_X);
        addch('*');
    }
    return win;
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
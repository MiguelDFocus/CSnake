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

int PLAYZONE_X = 126, PLAYZONE_Y = 50;

int main(void) {
	initscr(); // Start curses
    noecho(); // Avoid user inputs to appear on screen
    char input;
    bool game_over = false;
    int y = PLAYZONE_Y / 2, x = PLAYZONE_X / 2;

    WINDOW *main_window = draw_playzone();
    WINDOW *play_window = newwin(PLAYZONE_Y - 1, PLAYZONE_X - 1, PLAYZONE_Y - 1, PLAYZONE_X - 1);

    do {
        input = getch();
        switch (input) {
            case 'q':
            game_over = true;
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
        }
        wclear(play_window);
        move(y, x);
        addch('#');
        refresh();
    } while (!game_over);

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
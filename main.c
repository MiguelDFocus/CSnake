/*
Steps to create the game:
    - Start ncurses - Done
    - Have a endless loop with ncurses open - Done
    - Draw a white square to the screen - Done
    - Make the white square move 1 block by user input - Done
    - Make the square move forward automatically and only change direction if input is pressed -- Done
    - Add collision to the borders of the screen -- Done
    - Add death and break out of loop if collission is detected -- Done
    - Add food, make it spawn on random points of the screen -- Done
    - Add collision to the food, make it disappear on collision with snake -- Done
    - Keep track of the score
    - Increase length of snake when food is consumed
    - Add death when touching your own tail
*/


#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>

struct Food {
    int x;
    int y;
    bool eaten;
};

struct Snake {
    int x;
    int y;
    struct Snake *next_snake;
};

WINDOW *draw_playzone(void);
struct Snake create_snake(int x, int y);
struct Snake *get_tail(struct Snake *snake);
void increase_snake_length(struct Snake *snake, char direction);
void move_snake(WINDOW *play_window, struct Snake *snake, char direction, int *x, int *y);
struct Food create_food_particle(void);
void draw_food_particle(WINDOW *food_window, int x, int y);
bool window_border_touched(int x, int y);
bool is_input_correct(char input);
bool is_direction_opposite(char input, char direction);
void show_end_game_message(int start_x_position, int start_y_position, int score);

const int MAIN_WINDOW_COLS = 60, MAIN_WINDOW_ROWS = 15;
const int PLAYABLE_ZONE_COLS = MAIN_WINDOW_COLS - 1, PLAYABLE_ZONE_ROWS = MAIN_WINDOW_ROWS - 1;
const int PLAYABLE_ZONE_X_START = 1, PLAYABLE_ZONE_Y_START = 1;
const char DIRECTIONS [4] = {'w', 'a', 's', 'd'};

int main(void) {
    srand(time(0));
	initscr(); // Start curses
    noecho(); // Avoid user inputs to appear on screen
    curs_set(0);

    int start_x_position = PLAYABLE_ZONE_COLS / 2, start_y_position = PLAYABLE_ZONE_ROWS / 2; 
    int x = start_x_position, y = start_y_position; // Get the middle of the playable window
    int score = 0;
    char input; // Initialise input to be populated on loop
    char direction = 'd'; // Snake will start moving to the right when game starts

    
    WINDOW *main_window = draw_playzone(); // Draw a square to act as play zone
    WINDOW *play_window = newwin(PLAYABLE_ZONE_ROWS, PLAYABLE_ZONE_COLS, PLAYABLE_ZONE_X_START, PLAYABLE_ZONE_X_START);
    WINDOW *food_window = newwin(PLAYABLE_ZONE_ROWS, PLAYABLE_ZONE_COLS, PLAYABLE_ZONE_X_START, PLAYABLE_ZONE_X_START);
    nodelay(play_window, true); // Make listening for input non blocking
    
    struct Snake snake = create_snake(x, y);
    struct Food food_particle = create_food_particle();

    while ((input = wgetch(play_window)) != 'q') {
        // If a key was pressed, assign direction
        if (input != ERR && is_input_correct(input)) {
            if (!is_direction_opposite(input, direction)) {
                direction = input;
            }
        }
        // Create food particle if needed
        if (food_particle.eaten) {
            food_particle = create_food_particle();
        }
        
        if (x == food_particle.x && y == food_particle.y) {
            // TODO: Fix bug where food particle starts moving in same direction as snake
            score++;
            increase_snake_length(&snake, direction);
            food_particle.eaten = true;
        }

        draw_food_particle(food_window, food_particle.x, food_particle.y);
        move_snake(play_window, &snake, direction, &x, &y);
        
        // Finish game if border is touched
        if (window_border_touched(x, y)) {
            usleep(500000);
            werase(play_window);
            wrefresh(play_window);
            show_end_game_message(start_x_position, start_y_position, score);
            break;
        }
        usleep(150000);
    }

	endwin();
    // Destroy snake linked list to avoid memory leaks
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

    refresh();
    return win;
}

struct Snake create_snake(int x, int y) {
    struct Snake snake;
    snake.x = x;
    snake.y = y;
    snake.next_snake = NULL;
    return snake;
}

struct Snake *get_tail(struct Snake *snake) {
    while (snake->next_snake != NULL) {
        snake = snake->next_snake;
    }
    return snake;
}

void increase_snake_length(struct Snake *snake, char direction) {
    struct Snake *tail = get_tail(snake);
    if (tail == NULL) {
        return;
    }

    struct Snake *new_snake = malloc(sizeof(struct Snake));
    if (new_snake == NULL) {
        return;
    }
    
    tail->next_snake = new_snake;
    int x;
    int y;
    switch (direction) {
        case 'w':
        y = snake->y + 1;
        break;
        case 's':
        y = snake->y - 1;
        break;
        case 'a':
        x = snake->x + 1;
        break;
        case 'd':
        x = snake->x - 1;
        break;
        default:
        break;
    }
    *new_snake = create_snake(x, y);
}

void move_snake(WINDOW *play_window, struct Snake *snake, char direction, int *x, int *y) {
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
    snake->x = *x;
    snake->y = *y;
    wmove(play_window, *y, *x);
    waddch(play_window, '#');
}

struct Food create_food_particle(void) {
    int x = rand() % PLAYABLE_ZONE_COLS + 1;
    int y = rand() % PLAYABLE_ZONE_ROWS + 1;

    struct Food food_particle;
    food_particle.x = x;
    food_particle.y = y;
    food_particle.eaten = false;
    return food_particle;
}

void draw_food_particle(WINDOW *food_window, int x, int y) {
    wmove(food_window, y, x);
    waddch(food_window, '@');
    wrefresh(food_window);
}

bool window_border_touched(int x, int y) {
    // Upper and left bounds have to be checked "greeedily"
    // Due to the window being created 1 pixel down and right to not collide with main areaS
    if ((x < 0) | (x >= PLAYABLE_ZONE_COLS) | (y < 0) | (y >= PLAYABLE_ZONE_ROWS)) {
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

void show_end_game_message(int start_x_position, int start_y_position, int score) {
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

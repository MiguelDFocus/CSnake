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
    - Keep track of the score -- Done
    - Increase length of snake when food is consumed -- Done
    - Fix increasing size of snake -- Done
    - Draw whole snake -- Done
    - Make head change direction, then whole snake to follow one by one -- Done
    - Add death when touching your own tail -- Done
*/

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>

struct Food {
    int line;
    int col;
};

struct Snake {
    int line;
    int col;
    struct Snake *next_snake;
};

void draw_playing_zone(WINDOW *window);
void draw_score(WINDOW *window, int line_start, int col_start, int score);
void draw_end_game_message(WINDOW *play_window, int start_line, int start_col);
void draw_snake(WINDOW *window, struct Snake *snake);
struct Snake *create_snake(int line, int col);
struct Snake *get_tail(struct Snake *snake);
void increase_snake_length(struct Snake *snake, char direction);
void move_snake(WINDOW *play_window, struct Snake *snake, char direction, int *line, int *col);
struct Food create_food_particle(void);
void draw_food_particle(WINDOW *food_window, int line, int col);
bool window_border_touched(int line, int col);
bool tail_touched(struct Snake *snake, int line, int col);
bool is_input_correct(char input);
bool is_direction_opposite(char input, char direction);
void destroy_snake(struct Snake *snake);

const int MAIN_WINDOW_COLS = 39, MAIN_WINDOW_LINES = 15;
const int PLAYABLE_ZONE_COLS = MAIN_WINDOW_COLS - 2, PLAYABLE_ZONE_LINES = MAIN_WINDOW_LINES - 2;
const int PLAYABLE_ZONE_LINE_START = 1, PLAYABLE_ZONE_COL_START = 1;
const int SCORE_WINDOW_COLS = 15, SCORE_WINDOW_LINES = 5;
const int SCORE_ZONE_LINE_START = 1, SCORE_ZONE_COL_START = MAIN_WINDOW_COLS + 2;
const char DIRECTIONS [4] = {'w', 'a', 's', 'd'};

int main(void) {
    srand(time(0));
    initscr(); // Start curses
    noecho(); // Avoid user inputs to appear on screen
    curs_set(0);

    int start_line = PLAYABLE_ZONE_LINES / 2, start_col = PLAYABLE_ZONE_COLS / 2;
    int line = start_line, col = start_col; // Get the middle of the playable window
    int score = 0;
    char input; // Initialise input to be populated on loop
    char direction = 'd'; // Snake will start moving to the right when game starts

    WINDOW *main_window = newwin(MAIN_WINDOW_LINES, MAIN_WINDOW_COLS, 0, 0);
    draw_playing_zone(main_window); // Draw a square to act as play zone

    WINDOW *play_window = newwin(PLAYABLE_ZONE_LINES, PLAYABLE_ZONE_COLS, PLAYABLE_ZONE_LINE_START, PLAYABLE_ZONE_COL_START);
    WINDOW *food_window = newwin(PLAYABLE_ZONE_LINES, PLAYABLE_ZONE_COLS, PLAYABLE_ZONE_LINE_START, PLAYABLE_ZONE_COL_START);
    WINDOW *score_window = newwin(SCORE_WINDOW_LINES, SCORE_WINDOW_COLS, SCORE_ZONE_LINE_START, SCORE_ZONE_COL_START);
    nodelay(play_window, true); // Make listening for input non blocking
    
    struct Snake *snake = create_snake(line, col);
    struct Food food_particle = create_food_particle();

    while ((input = wgetch(play_window)) != 'q') {
        // If a key was pressed, assign direction
        if (input != ERR && is_input_correct(input)) {
            if (!is_direction_opposite(input, direction)) {
                direction = input;
            }
        }
        
        if (line == food_particle.line && col == food_particle.col) {
            score++;
            increase_snake_length(snake, direction);
            food_particle = create_food_particle();
        }

        move_snake(play_window, snake, direction, &line, &col);
        draw_snake(play_window, snake);
        draw_food_particle(food_window, food_particle.line, food_particle.col);
        draw_score(score_window, SCORE_ZONE_LINE_START, SCORE_ZONE_COL_START, score);
        
        // Finish game if border or own tail is touched
        if (window_border_touched(line, col) || tail_touched(snake, line, col)) {
            usleep(500000);
            draw_end_game_message(play_window, start_line, start_col);
            break;
        }
        usleep(150000);
    }

    endwin();
    // Destroy snake linked list to avoid memory leaks
    destroy_snake(snake);
    return 0;
}

void draw_playing_zone(WINDOW *window) {
    box(window, 0, 0);
    wrefresh(window);
}

void draw_food_particle(WINDOW *food_window, int line, int col) {
    wmove(food_window, line, col);
    waddch(food_window, '@');
    wrefresh(food_window);
}

void draw_score(WINDOW *window, int line_start, int col_start, int score) {
    werase(window);
    wrefresh(window);
    wmove(window, line_start, col_start);
    wprintw(window, "Score: %i", score);
    wrefresh(window);
}

void draw_end_game_message(WINDOW *play_window, int start_line, int start_col) {
    int base_string_length = 10;
    
    werase(play_window);
    wrefresh(play_window);
    wmove(play_window, start_line, start_col - (base_string_length / 2));
    wprintw(play_window, "Game over!");
    wrefresh(play_window);
    usleep(1500000);
}

void draw_snake(WINDOW *window, struct Snake *snake) {
    werase(window);
    while (snake != NULL) {
        wmove(window, snake->line, snake->col);
        waddch(window, '#');
        snake = snake->next_snake;
    }
    wrefresh(window);
}

struct Snake *create_snake(int line, int col) {
    struct Snake *snake = malloc(sizeof(struct Snake));
    if (snake == NULL) {
        return 0;
    }
    snake->line = line;
    snake->col = col;
    snake->next_snake = NULL;
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
    
    int line = tail->line;
    int col = tail->col;
    switch (direction) {
        case 'w':
            line++;
            break;
        case 's':
            line--;
            break;
        case 'a':
            col++;
            break;
        case 'd':
            col--;
            break;
        default:
            break;
    }
    tail->next_snake = create_snake(line, col);
}

void move_snake(WINDOW *play_window, struct Snake *snake, char direction, int *line, int *col) {
    /*
    Save current snake state
    Update current snake to previous snake state
    Update current snake to be next snake
    */
    int previous_line = snake->line;
    int previous_col = snake->col;

    switch (direction) {
       case 'w':
        (*line)--;
       break;
        case 's':
        (*line)++;
       break;
        case 'a':
        (*col)--;
       break;
       case 'd':
        (*col)++;
        break;
       default:
        break;
    }
    snake->line = *line;
    snake->col = *col;

    while ((snake = snake->next_snake) != NULL) {
        struct Snake tmp = *snake;

        snake->line = previous_line;
        snake->col = previous_col;

        previous_col = tmp.col;
        previous_line = tmp.line;
    }
}

struct Food create_food_particle(void) {
    int line = rand() % PLAYABLE_ZONE_LINES;
    int col = rand() % PLAYABLE_ZONE_COLS;

    struct Food food_particle;
    food_particle.line = line;
    food_particle.col = col;
    return food_particle;
}

bool window_border_touched(int line, int col) {
    // Upper and left bounds have to be checked "greeedily"
    // Due to the window being created 1 pixel down and right to not collide with main areaS
    if ((line < 0) || (line >= PLAYABLE_ZONE_LINES) || (col < 0) || (col >= PLAYABLE_ZONE_COLS)) {
        return true;
    }
    return false;
}

bool tail_touched(struct Snake *snake, int line, int col) {
    while ((snake = snake->next_snake) != NULL) {
        if (snake->col == col && snake->line == line) {
            return true;
        }
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

void destroy_snake(struct Snake *snake) {
    while (snake != NULL) {
        struct Snake *tmp = snake;
        snake = tmp->next_snake;
        free(tmp);
    }
}
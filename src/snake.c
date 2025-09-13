#include <config.h>
#include <snake.h>
#include <game.h>

#include <unistd.h>
#include <stdlib.h>

void draw_snake(struct Snake *snake, WINDOW *window) {
    struct Snake *current = snake;
    werase(window);
    while (current != NULL) {
        wmove(window, current->line, current->col);
        waddch(window, '#');
        current = current->next_snake;
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

void move_snake(struct GameState *game) {
    int previous_line = game->snake->line;
    int previous_col = game->snake->col;

    switch (game->direction) { 
        case 'w': game->line--; break;
        case 's': game->line++; break;
        case 'a': game->col--; break;
        case 'd': game->col++; break;
    }

    game->snake->line = game->line;
    game->snake->col = game->col;

    struct Snake *current = game->snake->next_snake;  // traverse without losing head
    while (current != NULL) {
        struct Snake tmp = *current;

        current->line = previous_line;
        current->col = previous_col;

        previous_line = tmp.line;
        previous_col = tmp.col;

        current = current->next_snake;
    }
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

void destroy_snake(struct Snake *snake) {
    while (snake != NULL) {
        struct Snake *tmp = snake;
        snake = tmp->next_snake;
        free(tmp);
    }
}

bool detect_food_collision(struct Snake *snake, struct Food food) {
    if (snake->line == food.line && snake->col == food.col) {
        return true;
    }
    return false;
}
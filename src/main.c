#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <config.h>
#include <game.h>
#include <snake.h>
#include <food.h>

int main(void) {
    srand(time(0));
    initscr(); // Start curses
    noecho(); // Avoid user inputs to appear on screen
    curs_set(0);

    struct GameState game = {
        .line = START_LINE,
        .col = START_COL,
        .score = 0,
        .direction = 'd', // Snake will start moving to the right when game starts
        .main_window = newwin(MAIN_WINDOW_LINES, MAIN_WINDOW_COLS, 0, 0),
        .play_window = newwin(PLAYABLE_ZONE_LINES, PLAYABLE_ZONE_COLS, PLAYABLE_ZONE_LINE_START, PLAYABLE_ZONE_COL_START),
        .food_window = newwin(PLAYABLE_ZONE_LINES, PLAYABLE_ZONE_COLS, PLAYABLE_ZONE_LINE_START, PLAYABLE_ZONE_COL_START),
        .score_window = newwin(SCORE_WINDOW_LINES, SCORE_WINDOW_COLS, SCORE_ZONE_LINE_START, SCORE_ZONE_COL_START),
    };

    draw_playing_zone(game.main_window); // Draw a square to act as play zone
    nodelay(game.play_window, true); // Make listening for input non blocking
    
    game.snake = create_snake(game.line, game.col);
    game.food = create_food_particle();

    while ((game.input = wgetch(game.play_window)) != 'q') {
        // If a key was pressed, assign direction
        if (game.input != ERR && is_input_correct(game.input)) {
            if (!is_direction_opposite(game.input, game.direction)) {
                game.direction = game.input;
            }
        }
        
        if (detect_food_collision(game.snake, game.food)) {
            game.score++;
            increase_snake_length(game.snake, game.direction);
            game.food = create_food_particle();
        }

        move_snake(&game);
        draw_snake(game.snake, game.play_window);
        draw_food_particle(game.food_window, game.food.line, game.food.col);
        draw_score(game.score_window, game.score);
        
        // Finish game if border or own tail is touched
        if (window_border_touched(game.line, game.col) || tail_touched(game.snake, game.line, game.col)) {
            usleep(END_GAME_SLEEP_MS);
            draw_end_game_message(game.play_window);
            break;
        }
        usleep(LOOP_SLEEP_MS);
    }

    endwin();
    // Destroy snake linked list to avoid memory leaks
    destroy_snake(game.snake);
    return 0;
}

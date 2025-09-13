#include "config.h"

const int MAIN_WINDOW_COLS = 39;
const int MAIN_WINDOW_LINES = 15;

const int PLAYABLE_ZONE_COLS = MAIN_WINDOW_COLS - 2;
const int PLAYABLE_ZONE_LINES = MAIN_WINDOW_LINES - 2;
const int PLAYABLE_ZONE_LINE_START = 1;
const int PLAYABLE_ZONE_COL_START = 1;

const int SCORE_WINDOW_COLS = 15;
const int SCORE_WINDOW_LINES = 5;
const int SCORE_ZONE_LINE_START = 1;
const int SCORE_ZONE_COL_START = MAIN_WINDOW_COLS + 2;

const int START_LINE = PLAYABLE_ZONE_LINES / 2;
const int START_COL = PLAYABLE_ZONE_COLS / 2;

const char DIRECTIONS [4] = {'w', 'a', 's', 'd'};

const int LOOP_SLEEP_MS = 100000;
const int END_GAME_SLEEP_MS = 500000;
#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <HAL/HAL.h>
#include <stdlib.h>
#include <HAL/HAL.h>
// #include <time.h>

#ifdef PE
#undef PE
#endif

#define NUM_SCORES 5
#define MIN_LINE_SPACING 12
#define MAX_LINE_SPACING 25
#define MAX_GRID 5*5
#define GRID_Y_START 23
#define GRID_Y_END 128
#define GRID_X_START 13
#define GRID_X_END 118
#define GRID_Y_RANGE (GRID_Y_END - GRID_Y_START)
#define GRID_X_RANGE (GRID_X_END - GRID_X_START)
#define SQUARE_SPACING 5
#define MAX_INIT_STATS 1000

// Small functional macros for convenience
#define SQR(x) ((x) * (x))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

typedef enum {BLACK, WHITE, RED, ORANGE, YELLOW, BLUE, GREEN} color_t;

typedef enum {MAIN, INSTRUCTIONS, HS, GRIDSELECT, GAME, GAMEOVER} screen_t;

typedef enum {OPTION_ONE, OPTION_TWO, OPTION_THREE, NUM_OPTIONS} option_t;

typedef enum {FLOOD, FAMINE, RAID, CALM, NUM_EVENTS} event_t;

typedef enum {IG, SP, AP, INVALID, NONE} action_t;

typedef enum {ASK_PE, INP_PE, ASK_TR, INP_TR, STATS_DONE} defstats_t;

typedef struct {
    action_t action;
    int8_t sq_idx;
} command_t;

typedef struct {
    uint8_t row, col;
    Graphics_Rectangle R;
    color_t color;
} square_t;


typedef struct {
    bool screen_changed;
    bool redraw;
    screen_t screen;
    color_t pen_color;
} display_t;


typedef struct {
    int PE;         
    int TR;
    int Default_PE;     // Starting stat
    int Default_TR;     // Starting stat
    unsigned year;
    uint8_t grid_length;            // 3 or 5
    square_t squares[MAX_GRID];     // The grid squares
    unsigned num_green;        // Number of green squares
    bool stats_change;         // Tracks change of stats
    bool new_cmd;               // Flag for new command
    event_t event;               // Tracks current event
    command_t cmd;               // Tracks current command
    unsigned high_scores[NUM_SCORES];       // Stores high scores
    defstats_t defstats;        // Tracks stage of initial stats input
} game_state_t;

/**
 * A helper function which increments a value with a maximum. If incrementing
 * the number causes the value to hit its maximum, the number wraps around
 * to 0.
 */
// Increments a value and wraps around to 0 if it exceeds maximum
uint32_t CircularIncrement(uint32_t value, uint32_t maximum);

// Converts the last 'count' characters in a string to an unsigned integer
unsigned chars_to_uint(const char *inp, uint8_t inp_size, uint8_t count);

// Prints a message to the UART interface
void UART_printMessage(HAL* hal_p, const char* msg);

// Gets the index of a square in the game grid, accounting for the center square
uint8_t get_square_idx(uint8_t idx, uint8_t grid_length);

// Initialize the graphics context with default settings
void InitGraphics(Graphics_Context *);

// Initialize fonts and set display orientation
void InitFonts();

// Main graphics initialization routine
void Initialize(Graphics_Context *);

// Update the pen color used for drawing graphics
void change_pen_color(Graphics_Context* g_sContext_p, color_t color);

// Draw the text elements for the main screen
void MainScreen_Text(Graphics_Context* g_sContext_p, uint8_t* j);

// Handle the main menu screen logic and display
void MainScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p);

// Instructions screen
void InstructionsScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p);

// High Scores Screen
void HSScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p, game_state_t* game_state_p);

// Draw the text elements for the main screen
void GridSelectScreen_Text(Graphics_Context* g_sContext_p, uint8_t* j);

// Handle the Grid Select screen logic and display
void GridSelectScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p, game_state_t* game_state_p);

// Handle the game screen logic and display
void GameScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p, game_state_t* game_state_p, uint32_t counter);

// Handle the game over screen logic and display
void GameOverScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p, game_state_t* game_state_p);

#endif
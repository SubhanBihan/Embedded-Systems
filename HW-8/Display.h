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
#define MAX_GRID 5*5
#define GRID_Y_START 24
#define GRID_Y_END 124
#define GRID_X_START 14
#define GRID_X_END 114
#define GRID_Y_RANGE (GRID_Y_END - GRID_Y_START)
#define GRID_X_RANGE (GRID_X_END - GRID_X_START)
#define SQUARE_SPACING 5

#define SQR(x) ((x) * (x))

// TODO: add colors if needed
typedef enum {BLACK, WHITE, RED, ORANGE, YELLOW, BLUE, GREEN} color_t;

typedef enum {MAIN, INSTRUCTIONS, HS, GAME, GAMEOVER} screen_t;

typedef enum {ONE, TWO, THREE, NUM_OPTIONS} option_t;

typedef enum {FLOOD, FAMINE, RAID, CALM, NUM_EVENTS} event_t;

typedef enum {IG, SP, AP, INVALID, NONE} action_t;
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
    unsigned year;
    uint8_t grid_length;       // 3 or 5
    square_t squares[MAX_GRID];
    bool stats_change;
    bool new_cmd;
    event_t event;
    command_t cmd;
} game_state_t;

/**
 * A helper function which increments a value with a maximum. If incrementing
 * the number causes the value to hit its maximum, the number wraps around
 * to 0.
 */
uint32_t CircularIncrement(uint32_t value, uint32_t maximum);

void UART_printMessage(HAL* hal_p, const char* msg);

uint8_t get_square_idx(uint8_t idx, uint8_t grid_length);

void InitGraphics(Graphics_Context *);
void InitFonts();
void Initialize(Graphics_Context *);

void change_pen_color(Graphics_Context* g_sContext_p, color_t color);

void MainScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p);
void InstructionsScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p);
void HSScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p, unsigned current_score);

void GameScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p, game_state_t* game_state_p);

#endif
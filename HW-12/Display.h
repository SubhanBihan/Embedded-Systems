#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "HAL/HAL.h"
#include <stdlib.h>
#include <math.h>

/// General
#define BACKGROUND_COLOR GRAPHICS_COLOR_BLACK
#define TEXT_COLOR GRAPHICS_COLOR_WHITE
#define ARROW_COLOR GRAPHICS_COLOR_GREEN
#define TITLE_DURATION_MS 2000

#define MIN_LINE_SPACING 12
#define MAX_LINE_SPACING 25
#define MIN_SECTION_GAP_ROWS 2

// Misc
#define SQRT2 0.707107

// Small functional macros for convenience
#define SQR(x) ((x) * (x))
#define AVG(a, b) (((a) + (b)) / 2)
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

// typedef enum {BLACK, WHITE, RED, ORANGE, YELLOW, BLUE, GREEN} color_t;

typedef enum {TITLE, MAIN, INSTRUCTIONS, HS, GAME, RESUME, GAMEOVER} screen_t;

typedef enum {OPTION_ONE, OPTION_TWO, NUM_OPTIONS} option_t;

typedef struct {
    // GFX gfx;
    bool screen_changed;
    bool redraw;
    screen_t screen;
} display_t;

typedef struct {
    uint32_t time;
} game_state_t;


// Increments a value and wraps around to 0 if it exceeds maximum
uint32_t CircularIncrement(uint32_t value, uint32_t maximum);

// Decrements a value and wraps around to maximum if it goes below 0
uint32_t CircularDecrement(uint32_t value, uint32_t maximum);

// Draw the Title Screen
void TitleScreen(HAL* hal_p, display_t* disp_p);

// // Draw the text elements for the main screen
// void MainScreen_Text(HAL* hal_p, uint8_t* j);

// Handle the main menu screen logic and display
void MainScreen(HAL* hal_p, display_t* disp_p);

// Instructions screen
void InstructionsScreen(HAL* hal_p, display_t* disp_p);

// // High Scores Screen
// void HSScreen(HAL* hal_p, display_t* disp_p, game_state_t* game_state_p);

// Game Screen
void GameScreen(HAL* hal_p, display_t* disp_p, game_state_t* game_state_p);

// // Game Over Screen
// void GameOverScreen(HAL* hal_p, display_t* disp_p, game_state_t* game_state_p);

// // Calculates score
// unsigned GetScore(int player_HP, unsigned game_time);

#endif
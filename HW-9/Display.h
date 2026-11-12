#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <HAL/HAL.h>
#include <stdlib.h>

#define BACKGROUND_COLOR GRAPHICS_COLOR_BLACK
#define TEXT_COLOR GRAPHICS_COLOR_WHITE
#define ARROW_COLOR GRAPHICS_COLOR_GREEN
#define TITLE_DURATION_MS 3000
#define MIN_SECTION_GAP_ROWS 2
#define NUM_SCORES 5
#define GRID_Y_START 0
#define GRID_Y_END 127
#define GRID_X_START 20
#define GRID_X_END 107
#define HPC_RADIUS 10
#define PLAYER_HPC_X (GRID_X_START - HPC_RADIUS)
#define PLAYER_HPC_Y (GRID_Y_END - HPC_RADIUS)
#define ENEMY_HPC_X (GRID_X_END + HPC_RADIUS)
#define ENEMY_HPC_Y (GRID_Y_START + HPC_RADIUS)
#define PLAYER_COLOR GRAPHICS_COLOR_AQUA
#define ENEMY_COLOR GRAPHICS_COLOR_RED
#define PLAYER_HPTEXT_X 0
#define PLAYER_HPTEXT_Y (PLAYER_HPC_Y - 3)
#define CHARACTER_REFRESH_MS 200
#define PROJECTILE_REFRESH_MS 100
#define CHARACTER_LENGTH 10
#define CHARACTER_DX 1
#define CHARACTER_DY 1
#define PROJECTILE_LENGTH 4
#define PROJECTILE_WIDTH 1
#define PROJECTILE_DX 2
#define PROJECTILE_DY 5
#define PLAYER_INIT_HP 100
#define PLAYER_INIT_XMIN 59
#define PLAYER_INIT_YMIN 59
#define MAX_PLAYER_BULLETS 5


#define MIN_LINE_SPACING 12
#define MAX_LINE_SPACING 25

// Small functional macros for convenience
#define SQR(x) ((x) * (x))
#define AVG(a, b) (((a) + (b)) / 2)
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

// typedef enum {BLACK, WHITE, RED, ORANGE, YELLOW, BLUE, GREEN} color_t;

typedef enum {TITLE, MAIN, INSTRUCTIONS, HS, GAME, GAMEOVER} screen_t;

typedef enum {OPTION_ONE, OPTION_TWO, OPTION_THREE, NUM_OPTIONS} option_t;

typedef struct {
    GFX gfx;
    bool screen_changed;
    bool redraw;
    screen_t screen;
} display_t;

typedef struct
{
    bool exists;            // Does the object still exist?
    bool stay;              // Is the object bounded to the screen?
    bool manual;            // Is motion manual (1) or autonomous (0)?
    bool mode;              // If autonomous, continuous (1) or sporadic (0)?
    int8_t up_down;         // Up (-1), None (0), Down (+1)
    int8_t left_right;      // Left (-1), None (0), Right (+1)
    uint8_t dx, dy;         // Number of pixels for vertical and horizontal movement
    SWTimer mTimer;         // Timer for movement
    uint16_t refresh_ms;    // Time interval for redrawing in ms
} motion_t;

typedef struct {
    Graphics_Rectangle body;
    uint32_t color;
    uint16_t HP;
    motion_t motion;
} character_t;

typedef struct {
    Graphics_Rectangle body;
    motion_t motion;
} projectile_t;

typedef struct
{
    character_t player, enemy;
    projectile_t player_bullet[MAX_PLAYER_BULLETS];
    uint32_t time;
    uint32_t high_scores[NUM_SCORES];
} game_state_t;


// Increments a value and wraps around to 0 if it exceeds maximum
uint32_t CircularIncrement(uint32_t value, uint32_t maximum);

// Decrements a value and wraps around to maximum if it goes below 0
uint32_t CircularDecrement(uint32_t value, uint32_t maximum);

// Function for controlling object (rectangle) motion
void MoveRectangle(HAL* hal_p, display_t* disp_p, Graphics_Rectangle* R, motion_t* motion_p);

// Test for rectangular overlap
bool IsOverlap(const Graphics_Rectangle* R1, const Graphics_Rectangle* R2);

// Draw the Title Screen
void TitleScreen(HAL* hal_p, display_t* disp_p);

// Draw the text elements for the main screen
void MainScreen_Text(display_t* disp_p, uint8_t* j);

// Handle the main menu screen logic and display
void MainScreen(HAL* hal_p, display_t* disp_p);

// Instructions screen
void InstructionsScreen(HAL* hal_p, display_t* disp_p);

// High Scores Screen
void HSScreen(HAL* hal_p, display_t* disp_p);

// Game Screen
void GameScreen(HAL* hal_p, display_t* disp_p, game_state_t* game_state_p);

#endif
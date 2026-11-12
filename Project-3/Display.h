#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "HAL/HAL.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

// General
#define TITLE_DURATION_MS 2000

// Game aspects
#define CURSOR_STARTING_PRICE 2.0
#define GRANDMA_STARTING_PRICE 2.0
#define CURSOR_PRICE_MULT 2.0
#define GRANDMA_PRICE_MULT 5.0
#define CURSOR_STARTING_CPI 2.0
#define GRANDMA_STARTING_CPI 10.0
#define CURSOR_UPG_STARTING_PRICE 1.0
#define GRANDMA_UPG_STARTING_PRICE 2.0
#define CURSOR_UPG_PRICE_MULT 5.0
#define GRANDMA_UPG_PRICE_MULT 10.0
#define CURSOR_UPG_CPI_MULT 2.0
#define GRANDMA_UPG_CPI_MULT 5.0
#define CURSOR_INTERVAL_MS 1000
#define GRANDMA_INTERVAL_MS 5000

// For BigNum
#define BN_BASE_MIN 1.0
#define BN_BASE_MAX 1000.0
#define BN_BASE_FACTOR 1000.0

// For LCD and Text
#define MIN_LINE_SPACING 12
#define MAX_LINE_SPACING 25
#define MIN_SECTION_GAP_ROWS 2
#define TEXT_BUFFER_LENGTH 25
#define TEXT_MIN_COL 1
#define COOKIES_ROW 1
#define CPS_ROW MODE_ROW
#define MODE_ROW 2
#define BUILDINGS_DESC_ROW 3
#define CURSOR_ROW 4
#define GRANDMA_ROW (CURSOR_ROW + 3)
#define UPGRADES_HEADER_ROW (GRANDMA_ROW + 3)
#define UPGRADES_DESC_ROW (UPGRADE_ROW + 1)
#define UPGRADE_ROW (UPGRADES_HEADER_ROW + 1)
#define STATS_COL 12
#define INDENT_COL 3
#define COOKIES_COL 9
#define CPS_TEXT_COL (MODE_SELL_COL + 5)
#define CPS_STATS_COL (CPS_TEXT_COL + 4)
#define MODE_BUY_COL TEXT_MIN_COL
#define MODE_SEP_COL (MODE_BUY_COL + 3)
#define MODE_SELL_COL (MODE_SEP_COL + 1)
#define UPGRADE_COL 8
#define UPGRADE_DESC_COL 1
#define LEFT_ARROW_COL 3
#define RIGHT_ARROW_COL 18


// Small functional macros for convenience
#define SQR(x) ((x) * (x))
#define AVG(a, b) (((a) + (b)) / 2)
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

// Screen type
typedef enum {TITLE, MAIN, INSTRUCTIONS, GAME} screen_t;

// Options for menus
typedef enum {OPTION_ONE, OPTION_TWO, NUM_OPTIONS} option_t;

// Game mode
typedef enum {BUY, SELL} mode_t;

// Struct for big numbers
typedef struct {
    double base;       // always in [1, 1000)
    uint8_t mag;       // 0 = none, 1 = k, 2 = M, 3 = B, 4 = T, 5 = Q, 6 = q
} BigNum;

// Struct for screen info
typedef struct {
    bool screen_changed;
    bool redraw;
    screen_t screen;
} display_t;

// Struct for game state
typedef struct {
    BigNum cookies;
    mode_t mode;
    int16_t cursor_num, grandma_num;
    int16_t cursor_lvl, grandma_lvl;
    BigNum cursor_bld_cost, grandma_bld_cost;
    BigNum cursor_bld_expense, grandma_bld_expense;
    BigNum cursor_upg_cost, grandma_upg_cost;
    BigNum cursor_upg_expense, grandma_upg_expense;
    BigNum cursor_cpi, grandma_cpi, cps;
} game_state_t;


// Normalization functions
void normalize_v(BigNum *x);
BigNum normalize_r(BigNum x);

// Arithmetic functions
void addsub_v(BigNum* a, BigNum b, bool subtract);     // 0 = add, 1 = sub
BigNum addsub_r(BigNum a, BigNum b, bool subtract);
void mul_v(BigNum* a, const BigNum* b);
BigNum mul_r(BigNum a, BigNum b);
void muldiv_v(BigNum* a, const double b, bool divide);
BigNum muldiv_r(BigNum a, const double b, bool divide);

// Comparison functions
bool GE_BigNum(const BigNum* a, const BigNum* b);

// String format function
void Format_BigNum(const BigNum* x, char* out, size_t maxlen);

// Calculate CPS
BigNum GetCPS(const game_state_t* gs_p);

// // Print cookies
// void PrintCookies(HAL* hal_p, game_state_t* gs_p);

// Increments a value and wraps around to 0 if it exceeds maximum
uint32_t CircularIncrement(uint32_t value, uint32_t maximum);

// Decrements a value and wraps around to maximum if it goes below 0
uint32_t CircularDecrement(uint32_t value, uint32_t maximum);


// Draw the Title Screen
void TitleScreen(HAL* hal_p, display_t* disp_p);

// Handle the main menu screen logic and display
void MainScreen(HAL* hal_p, display_t* disp_p);

// Instructions screen
void InstructionsScreen(HAL* hal_p, display_t* disp_p);

// Game Screen
void GameScreen(HAL* hal_p, display_t* disp_p, game_state_t* gs_p);

#endif
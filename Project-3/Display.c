#include "Display.h"

void normalize_v(BigNum *x) {
    if (!(x->base > 0.0)) { x->base = 0.0; x->mag = 0; return; }
    while (x->base >= BN_BASE_MAX) { x->base /= BN_BASE_FACTOR; x->mag++; }
    while (x->base < BN_BASE_MIN && x->mag > 0) { x->base *= BN_BASE_FACTOR; x->mag--; }
}

BigNum normalize_r(BigNum x) {
    normalize_v(&x);
    return x;
}

void addsub_v(BigNum* a, BigNum b, bool subtract) {
    // align magnitudes: make b same mag as a (or vice versa) by always bringing the smaller mag up
    while (b.mag < a->mag) { b.base /= BN_BASE_FACTOR; b.mag++; }
    while (a->mag < b.mag) { a->base /= BN_BASE_FACTOR; a->mag++; }

    if (subtract) a->base -= b.base;
    else         a->base += b.base;

    normalize_v(a);
}

BigNum addsub_r(BigNum a, BigNum b, bool add_or_sub) {
    addsub_v(&a, b, add_or_sub);
    return a;
}

void mul_v(BigNum* a, const BigNum* b) {
    a->base *= b->base;
    a->mag += b->mag;
    normalize_v(a);
}

BigNum mul_r(BigNum a, BigNum b) {
    mul_v(&a, &b);
    return a;
}

void muldiv_v(BigNum* a, const double b, bool divide) {
    a->base *= (divide ? 1.0/b : b);
    normalize_v(a);
}

BigNum muldiv_r(BigNum a, const double b, bool divide) {
    muldiv_v(&a, b, divide);
    return a;
}

bool GE_BigNum(const BigNum* a, const BigNum* b)
{
    // if (a->mag != b->mag) return a->mag > b->mag;
    // return a->base >= b->base;
    return (a->mag > b->mag) || (a->mag == b->mag && a->base >= b->base);
}

// void Format_BigNum(const BigNum* x, char* out, size_t maxlen) {
//     static const char* SUFFIXES[] = { "", "k", "M", "B", "T", "Q", "q" };
//     if (x->mag == 0) {
//         snprintf(out, maxlen, "%.0f", x->base);
//     } else if (x->base >= 100.0) {
//         snprintf(out, maxlen, "%.0f%s", x->base, SUFFIXES[x->mag]);
//     } else if (x->base >= 10.0) {
//         snprintf(out, maxlen, "%.1f%s", x->base, SUFFIXES[x->mag]);
//     } else {
//         snprintf(out, maxlen, "%.2f%s", x->base, SUFFIXES[x->mag]);
//     }
// }

void Format_BigNum(const BigNum* x, char* out, size_t maxlen) {
    static const char* SUFFIXES[] = { "", "k", "M", "B", "T", "Q", "q" };

    int whole = (int)x->base;
    int frac;

    if (x->mag == 0) {
        snprintf(out, maxlen, "%3d ", whole);
    } else if (x->base >= 100.0) {
        snprintf(out, maxlen, "%d%s", whole, SUFFIXES[x->mag]);
    } else if (x->base >= 10.0) {
        frac = (int)((x->base - whole) * 10);
        snprintf(out, maxlen, "%d.%1d%s", whole, frac, SUFFIXES[x->mag]);
    } else {
        frac = (int)((x->base - whole) * 100);
        snprintf(out, maxlen, "%d.%02d%s", whole, frac, SUFFIXES[x->mag]);
    }
}

BigNum GetCPS(const game_state_t* gs_p) {
    // temp copies
    BigNum total_cursor_cps = gs_p->cursor_cpi;
    BigNum total_grandma_cps = gs_p->grandma_cpi;

    // scale per-unit CPI by counts
    // multiply base by integer count then normalize
    total_cursor_cps.base *= (double)gs_p->cursor_num;
    normalize_v(&total_cursor_cps);

    total_grandma_cps.base *= (double)gs_p->grandma_num;
    // Grandma triggers every 5s while cursor every 1s, so convert grandma-per-5s to per-1s
    total_grandma_cps.base /= (double)(GRANDMA_INTERVAL_MS / CURSOR_INTERVAL_MS);
    normalize_v(&total_grandma_cps);

    // add the two
    return addsub_r(total_cursor_cps, total_grandma_cps, false);
}

void PrintCookies(HAL* hal_p, game_state_t* gs_p) {
    char BNbuffer[TEXT_BUFFER_LENGTH];
    Format_BigNum(&gs_p->cookies, BNbuffer, TEXT_BUFFER_LENGTH);
    GFX_print(&hal_p->gfx, BNbuffer, COOKIES_ROW, COOKIES_COL);
}

void PrintCPS(HAL* hal_p, game_state_t* gs_p) {
    char BNbuffer[TEXT_BUFFER_LENGTH];
    gs_p->cps = GetCPS(gs_p);
    Format_BigNum(&gs_p->cps, BNbuffer, TEXT_BUFFER_LENGTH);
    GFX_print(&hal_p->gfx, BNbuffer, CPS_ROW, CPS_STATS_COL);
}

void PrintMode(HAL* hal_p, game_state_t* gs_p) {
    static const char* Mode[] = {"Buy", "Sell"};
    GFX_print(&hal_p->gfx, Mode[gs_p->mode], MODE_ROW, gs_p->mode == BUY ? MODE_BUY_COL : MODE_SELL_COL);
    GFX_setForeground(&hal_p->gfx, TEXT_DARK_COLOR);
    GFX_print(&hal_p->gfx, Mode[1 - gs_p->mode], MODE_ROW, gs_p->mode == BUY ? MODE_SELL_COL : MODE_BUY_COL);
    GFX_setForeground(&hal_p->gfx, TEXT_LIT_COLOR);
}

void PrintBuildingNames(HAL* hal_p, const option_t bld_opn) {
    static const char* Buildings[] = {"Cursor ", "Grandma"};

    GFX_print(&hal_p->gfx, Buildings[bld_opn], bld_opn == OPTION_ONE ? CURSOR_ROW : GRANDMA_ROW, INDENT_COL);
    GFX_setForeground(&hal_p->gfx, TEXT_DARK_COLOR);
    GFX_print(&hal_p->gfx, Buildings[1 - bld_opn], bld_opn == OPTION_ONE ? GRANDMA_ROW : CURSOR_ROW, INDENT_COL);
    GFX_setForeground(&hal_p->gfx, TEXT_LIT_COLOR);
}

void PrintCursor(HAL* hal_p, game_state_t* gs_p) {
    char BNbuffer[TEXT_BUFFER_LENGTH];
    snprintf(BNbuffer, TEXT_BUFFER_LENGTH, "%3d", gs_p->cursor_num);
    GFX_print(&hal_p->gfx, BNbuffer, CURSOR_ROW, STATS_COL);
    Format_BigNum(&gs_p->cursor_bld_cost, BNbuffer, TEXT_BUFFER_LENGTH);
    GFX_print(&hal_p->gfx, BNbuffer, CURSOR_ROW + 1, STATS_COL);
}

void PrintGrandma(HAL* hal_p, game_state_t* gs_p) {
    char BNbuffer[TEXT_BUFFER_LENGTH];
    snprintf(BNbuffer, TEXT_BUFFER_LENGTH, "%3d", gs_p->grandma_num);
    GFX_print(&hal_p->gfx, BNbuffer, GRANDMA_ROW, STATS_COL);
    Format_BigNum(&gs_p->grandma_bld_cost, BNbuffer, TEXT_BUFFER_LENGTH);
    GFX_print(&hal_p->gfx, BNbuffer, GRANDMA_ROW + 1, STATS_COL);
}

void PrintUpgDesc(HAL* hal_p, game_state_t* gs_p, const option_t upg_opn, const int16_t* lvl_p, const bool upg_isviable) {
    static const char* Buildings[] = {"Cursor ", "Grandma"};
    char upg_desc[TEXT_BUFFER_LENGTH];
    snprintf(upg_desc, TEXT_BUFFER_LENGTH, "%s lvl %-3d>%-3d", Buildings[upg_opn], *lvl_p, *lvl_p + ((gs_p->mode == BUY) ? 1 : (*lvl_p > 0) ? -1 : 0));

    if (!upg_isviable) GFX_setForeground(&hal_p->gfx, TEXT_DARK_COLOR);
    GFX_print(&hal_p->gfx, upg_desc, UPGRADES_DESC_ROW, UPGRADE_DESC_COL);
    if (!upg_isviable) GFX_setForeground(&hal_p->gfx, TEXT_LIT_COLOR);
}

void PrintUpgCost(HAL* hal_p, BigNum* upg_cost_p) {
    char upg_cost_str[TEXT_BUFFER_LENGTH];
    Format_BigNum(upg_cost_p, upg_cost_str, TEXT_BUFFER_LENGTH);
    GFX_print(&hal_p->gfx, upg_cost_str, UPGRADE_ROW, UPGRADE_COL);
}

void DrawHarrows(HAL* hal_p, const option_t upg_opn) {
    static const char* HArrows[] = {"<", ">"};

    GFX_setForeground(&hal_p->gfx, TEXT_DARK_COLOR);
    GFX_print(&hal_p->gfx, HArrows[upg_opn], UPGRADE_ROW, upg_opn == OPTION_ONE ? LEFT_ARROW_COL : RIGHT_ARROW_COL);
    GFX_setForeground(&hal_p->gfx, TEXT_LIT_COLOR);
    GFX_print(&hal_p->gfx, HArrows[1 - upg_opn], UPGRADE_ROW, upg_opn == OPTION_ONE ? RIGHT_ARROW_COL : LEFT_ARROW_COL);
}

void DrawVarrow(HAL* hal_p, const option_t bld_opn, const bool bld_isviable) {
    if (!bld_isviable) GFX_setForeground(&hal_p->gfx, TEXT_DARK_COLOR);
    GFX_print(&hal_p->gfx, "> ",  bld_opn == OPTION_ONE ? CURSOR_ROW : GRANDMA_ROW, TEXT_MIN_COL);
    GFX_print(&hal_p->gfx, "  ", bld_opn == OPTION_TWO ? CURSOR_ROW : GRANDMA_ROW, TEXT_MIN_COL);
    if (!bld_isviable) GFX_setForeground(&hal_p->gfx, TEXT_LIT_COLOR);
}

uint32_t CircularIncrement(uint32_t value, uint32_t maximum) {
  return (value + 1) % maximum;
}

uint32_t CircularDecrement(uint32_t value, uint32_t maximum) {
    return value = (value + maximum - 1) % maximum;
}

void TitleScreen(HAL* hal_p, display_t* disp_p) {
    static SWTimer TitleTimer;  // Timer for Title screen
    

    if (disp_p->screen_changed) {   // Only do these when first entering title screen (function)
        // Clear the display
        GFX_clear(&hal_p->gfx);
        
        // Print Game name
        GFX_print(&hal_p->gfx, "Cookie Clicker", 2, 4);

        // Print Dev's name
        GFX_print(&hal_p->gfx, "Subhan Bihan", 4, 4);

        // Construct and start the timer
        TitleTimer = SWTimer_construct(TITLE_DURATION_MS);
        
        SWTimer_start(&TitleTimer);

        // Turn off the screen_changed flag
        disp_p->screen_changed = false;
    }

    // When the timer expries, change settings to move to Main Menu screen
    if (SWTimer_expired(&TitleTimer)) {
    // if (hal_p->buttons.AnyButtonTap) {
        disp_p->screen = MAIN;
        disp_p->screen_changed = true;
    }
}


void MainScreen(HAL* hal_p, display_t* disp_p) {
    static option_t option_num;
    static uint8_t j;
    static const char* lines[] = {"  Play Game", "  Instructions"};
    static const uint8_t num_lines = sizeof(lines) / sizeof(char*);

    // Upon entry into function
    if (disp_p->screen_changed) {
        // MainScreen_Text(&hal_p->gfx.context, &j);
        Graphics_clearDisplay(&hal_p->gfx.context);
        GFX_setForeground(&hal_p->gfx, TEXT_LIT_COLOR);   // Text color

        uint8_t i = 0;

        Graphics_drawString(&hal_p->gfx.context, (int8_t *)"MENU", -1, 5, MIN_LINE_SPACING * ++i, true);
        j = i;

        for (i = 0; i - j < num_lines; i++) {
            Graphics_drawString(&hal_p->gfx.context, lines[i - j], -1, 5, MIN_LINE_SPACING * (i + 1), true);
        }

        disp_p->screen_changed = false;
        disp_p->redraw = true;
        option_num = OPTION_ONE;
    }

    // Move cursor/arrow
    if (hal_p->JS.TappedY != CENTERY) {
        option_num = hal_p->JS.TappedY == UP ? CircularDecrement(option_num, NUM_OPTIONS) : CircularIncrement(option_num, NUM_OPTIONS);
        disp_p->redraw = true;
    }

    // Change the option arrow
    if (disp_p->redraw) {
        uint8_t k; for (k = 0; k < NUM_OPTIONS; k++) {
            if (k == option_num)
                Graphics_drawString(&hal_p->gfx.context, (int8_t *)">", -1, 5, MIN_LINE_SPACING * (j + k + 1), true);
            else
                Graphics_drawString(&hal_p->gfx.context, (int8_t *)"  ", -1, 5, MIN_LINE_SPACING * (j + k + 1), true);
        }
        disp_p->redraw = false;
    }

    // Select the indicated option
    if (hal_p->buttons.JSBtapped) {
        disp_p->screen = option_num == OPTION_ONE ? GAME : INSTRUCTIONS;
        disp_p->screen_changed = true;
    }
}


void InstructionsScreen(HAL* hal_p, display_t* disp_p) {
    // Instructions text - CHANGE THE LINES
    static const char* lines[] = {"LB1/LB2: Switch Mode", "Buy(B)/Sell(S)", "JSB: +1 Cookie", "JS L/R: See Upgrade", "JS U/D: See Building", "BB1: B/S Building", "BB2: B/S Upgrade"};
    static const uint8_t num_lines = sizeof(lines) / sizeof(char*);

    if (disp_p->screen_changed) {   // Only do these when first entering main screen (function)
        // MainScreen_Text(&hal_p->gfx.context, &j);

        // Clear the display
        GFX_clear(&hal_p->gfx);

        // Set foreground color
        GFX_setForeground(&hal_p->gfx, TEXT_LIT_COLOR);

        uint8_t i = 1;  // Tracking row of current line

        // Print Screen name
        GFX_print(&hal_p->gfx, "INSTRUCTIONS", i, 1);
        i += MIN_SECTION_GAP_ROWS;

        // Print instructions
        i = GFX_printTextRows(&hal_p->gfx, lines, num_lines, i, 1);
        i += MIN_SECTION_GAP_ROWS;

        // Print navigations instructions
        GFX_print(&hal_p->gfx, "JSB: Go Back", i, 1);

        // Set flags to stay on this screen
        disp_p->screen_changed = false;
    }

    if (hal_p->buttons.JSBtapped) {
        disp_p->screen = MAIN;
        disp_p->screen_changed = true;
    }
}

void Building_BS(game_state_t* gs_p, int16_t *bld_num_p, BigNum *bld_cost_p, BigNum *bld_expense_p, const option_t bld_opn) {
    if (gs_p->mode == BUY) {
        addsub_v(&gs_p->cookies, *bld_cost_p, true);                                                        // Subtract the building cost from cookies
        (*bld_num_p)++;                                                                                     // Add a building
        *bld_expense_p = *bld_cost_p;                                                                   // Expense = Current cost
        muldiv_v(bld_cost_p, (bld_opn == OPTION_ONE) ? CURSOR_PRICE_MULT : GRANDMA_PRICE_MULT, false);      // Scale up both the (next) cost
    }
    else {
        addsub_v(&gs_p->cookies, *bld_expense_p, false);                                                    // Add the building expense to cookies
        (*bld_num_p)--;                                                                                     // Subtract a building
        *bld_cost_p = (*bld_num_p > 0) ? *bld_expense_p : (BigNum) {(bld_opn == OPTION_ONE) ? CURSOR_STARTING_PRICE : GRANDMA_STARTING_PRICE, 0};                                                                   // Cost = Current expense
        *bld_expense_p = (*bld_num_p > 0) ? muldiv_r(*bld_expense_p, (bld_opn == OPTION_ONE) ? CURSOR_PRICE_MULT : GRANDMA_PRICE_MULT, true) : (BigNum) {0.0, 0};     // Scale down expense
    }
}

void Upgrade_BS(game_state_t* gs_p, int16_t *lvl_p, BigNum *upg_cost_p, BigNum *upg_expense_p, BigNum *cpi_p, bool* cursor_lvl_fch, bool* grandma_lvl_fch, const option_t upg_opn) {
    if (gs_p->mode == BUY) {
            addsub_v(&gs_p->cookies, *upg_cost_p, true);                                                        // Subtract the upgrade cost from cookies
            (*lvl_p)++;                                                                                         // Add a level
            *upg_expense_p = *upg_cost_p;                                                                       // Expense = Current cost
            muldiv_v(upg_cost_p, (upg_opn == OPTION_ONE) ? CURSOR_UPG_PRICE_MULT : GRANDMA_UPG_PRICE_MULT, false);      // Scale up both the (next) cost
            muldiv_v(cpi_p, (upg_opn == OPTION_ONE) ? CURSOR_UPG_CPI_MULT : GRANDMA_UPG_CPI_MULT, false);         // Scale up the CPI

            if (upg_opn == OPTION_ONE) *cursor_lvl_fch = true;   else *grandma_lvl_fch = true;
        }
        else {
            addsub_v(&gs_p->cookies, *upg_expense_p, false);                                                    // Add the building expense to cookies
            (*lvl_p)--;                                                                                     // Subtract a building
            *upg_cost_p = (*lvl_p > 0) ? *upg_expense_p : (BigNum) {(upg_opn == OPTION_ONE) ? CURSOR_UPG_STARTING_PRICE : GRANDMA_UPG_STARTING_PRICE, 0};                                                                   // Cost = Current expense
            *upg_expense_p = (*lvl_p > 0) ? muldiv_r(*upg_expense_p, (upg_opn == OPTION_ONE) ? CURSOR_UPG_PRICE_MULT : GRANDMA_UPG_PRICE_MULT, true) : (BigNum) {0.0, 0};     // Scale down expense
            muldiv_v(cpi_p, (upg_opn == OPTION_ONE) ? CURSOR_UPG_CPI_MULT : GRANDMA_UPG_CPI_MULT, true);         // Scale down the CPI
        }
}

void Init_gs(game_state_t* gs_p) {
    gs_p->cookies = gs_p->grandma_bld_expense = gs_p->cursor_bld_expense = gs_p->grandma_upg_expense = gs_p->cursor_upg_expense = gs_p->cps = (BigNum) {0.0, 0};
    gs_p->cursor_num = gs_p->grandma_num = gs_p->cursor_lvl = gs_p->grandma_lvl = 0;
    gs_p->cursor_cpi = (BigNum) {CURSOR_STARTING_CPI, 0}; gs_p->grandma_cpi = (BigNum) {GRANDMA_STARTING_CPI, 0};
    gs_p->cursor_bld_cost = (BigNum) {CURSOR_STARTING_PRICE, 0}; gs_p->grandma_bld_cost = (BigNum) {GRANDMA_STARTING_PRICE, 0};
    gs_p->cursor_upg_cost = (BigNum) {CURSOR_UPG_STARTING_PRICE, 0}; gs_p->grandma_upg_cost = (BigNum) {GRANDMA_UPG_STARTING_PRICE, 0};
    gs_p->mode = BUY;

    TurnOn_BLB();   TurnOff_BLR();        // Cursor = BLB, Grandma = BLR
}

void GameScreen_Text(HAL* hal_p) {
    GFX_print(&hal_p->gfx, "Cookies",   COOKIES_ROW, TEXT_MIN_COL);
    GFX_print(&hal_p->gfx, "CPS",       CPS_ROW, CPS_TEXT_COL);
    GFX_print(&hal_p->gfx, "|",         MODE_ROW, MODE_SEP_COL);
    GFX_print(&hal_p->gfx, "BUILDINGS", BUILDINGS_DESC_ROW, TEXT_MIN_COL);
    GFX_print(&hal_p->gfx, "Cost",    CURSOR_ROW + 1, INDENT_COL);
    GFX_print(&hal_p->gfx, "Cost",    GRANDMA_ROW + 1, INDENT_COL);
    GFX_print(&hal_p->gfx, "UPGRADES",  UPGRADES_HEADER_ROW, TEXT_MIN_COL);
}

void GameScreen(HAL* hal_p, display_t* disp_p, game_state_t* gs_p) {
    // static const char* Buildings[] = {"Cursor ", "Grandma"};
    static note_t cursor_note, grandma_note;    static option_t bld_opn = OPTION_ONE, upg_opn = OPTION_ONE;
    static int16_t *bld_num_p, *upg_num_p, *lvl_p;  static BigNum *bld_cost_p, *bld_expense_p, *upg_cost_p, *upg_expense_p, *cpi_p;   static SWTimer cursor_timer, grandma_timer, buzzer_timer;

    static bool bld_isviable = false, bld_wasviable = false, upg_isviable = false, cursor_lvl_fch = true, grandma_lvl_fch = true;      // Used to track B/S viability first change (increment) of upgrade levels
    bool cookies_changed = false, cpi_changed = false, cps_changed = false, mode_changed = false, varrow_changed = false, harrow_changed = false, cursor_changed = false, grandma_changed = false;

    if (disp_p->screen_changed) {   // Only do these when first entering main screen (function)
        // Clear the display
        GFX_clear(&hal_p->gfx);

        // Initialisation and Fixed texts
        Init_gs(gs_p);  GameScreen_Text(hal_p);

        cookies_changed = cpi_changed = cps_changed = mode_changed = varrow_changed = harrow_changed = cursor_changed = grandma_changed = true;

        // Construct upgrade music notes
        cursor_note = Note_Construct(CURSOR_NOTE, BUZZER_DURATION_MS, BUZZER_DUTY_CYCLE_FRACTION);  grandma_note = Note_Construct(GRANDMA_NOTE, BUZZER_DURATION_MS, BUZZER_DUTY_CYCLE_FRACTION);

        // Construct and start the CPI timers
        cursor_timer = SWTimer_construct(CURSOR_INTERVAL_MS);   grandma_timer = SWTimer_construct(GRANDMA_INTERVAL_MS);
        SWTimer_start(&cursor_timer);                           SWTimer_start(&grandma_timer);

        // Set flags to stay on this screen
        disp_p->screen_changed = false;     disp_p->redraw = true;
    }

    // JSB tapped: +1 Cookie
    if (hal_p->buttons.JSBtapped) {
        // gs_p->cookies.base++;
        // normalize_v(&gs_p->cookies);
        addsub_v(&gs_p->cookies, (BigNum){1.0, 0}, false);      cookies_changed = true;
    }

    // Add Cursor CPI every 1s
    if (SWTimer_expired(&cursor_timer)) {
        if (gs_p->cursor_num > 0) {
            addsub_v(&gs_p->cookies, muldiv_r(gs_p->cursor_cpi, gs_p->cursor_num, false), false);       cookies_changed = true;
        }   SWTimer_start(&cursor_timer);
    }

    // Add Grandma CPI every 5s
    if (SWTimer_expired(&grandma_timer)) {
        if (gs_p->grandma_num > 0) {
            addsub_v(&gs_p->cookies, muldiv_r(gs_p->grandma_cpi, gs_p->grandma_num, false), false);     cookies_changed = true;
        }   SWTimer_start(&grandma_timer);
    }

    // Change Mode
    if (hal_p->buttons.LB1tapped || hal_p->buttons.LB2tapped) {
        gs_p->mode = 1 - gs_p->mode;    mode_changed = true;
    }

    // Move vertical cursor/arrow
    if (hal_p->JS.TappedY != CENTERY) {         Toggle_BLB();   Toggle_BLR();       // Toggle indicator light color
        bld_opn = (hal_p->JS.TappedY == UP) ? CircularDecrement(bld_opn, NUM_OPTIONS) : CircularIncrement(bld_opn, NUM_OPTIONS);    varrow_changed = true;
    }

    // Change pointers to current building specs
    if (varrow_changed) {       // Building selection changed if this is true (at this point)
        if (bld_opn == OPTION_ONE) {    bld_num_p = &gs_p->cursor_num; bld_cost_p = &gs_p->cursor_bld_cost; bld_expense_p = &gs_p->cursor_bld_expense;
        }
        else {                          bld_num_p = &gs_p->grandma_num; bld_cost_p = &gs_p->grandma_bld_cost; bld_expense_p = &gs_p->grandma_bld_expense;
        }

        // Also print building names (selected - lit)
        PrintBuildingNames(hal_p, bld_opn);
    }

    // Change horizontal option
    if (hal_p->JS.TappedX != CENTERX) {
        upg_opn = (hal_p->JS.TappedX == LEFT) ? CircularDecrement(upg_opn, NUM_OPTIONS) : CircularIncrement(upg_opn, NUM_OPTIONS);  harrow_changed = true;
    }

    // Change pointers to current Upgrade specs
    if (harrow_changed) {       // Upgrade selection changed if this is true (at this point)
        if (upg_opn == OPTION_ONE) {    lvl_p = &gs_p->cursor_lvl; upg_num_p = &gs_p->cursor_num; upg_cost_p = &gs_p->cursor_upg_cost; upg_expense_p = &gs_p->cursor_upg_expense; cpi_p = &gs_p->cursor_cpi;
        }
        else {  lvl_p = &gs_p->grandma_lvl; upg_num_p = &gs_p->grandma_num; upg_cost_p = &gs_p->grandma_upg_cost; upg_expense_p = &gs_p->grandma_upg_expense; cpi_p = &gs_p->grandma_cpi;
        }
    }

    
    // Buy/Sell Building
    if (hal_p->buttons.BB1tapped && bld_isviable) {     Building_BS(gs_p, bld_num_p, bld_cost_p, bld_expense_p, bld_opn);
        // NOTE: CPI is per building unit, so is unchanged by building number
        cookies_changed = true; cps_changed = true;  cursor_changed = (bld_opn == OPTION_ONE); grandma_changed = (bld_opn == OPTION_TWO);      // Set flags
    }

    // Buy/Sell Upgrade
    if (hal_p->buttons.BB2tapped && upg_isviable) {
        Upgrade_BS(gs_p, lvl_p, upg_cost_p, upg_expense_p, cpi_p, &cursor_lvl_fch, &grandma_lvl_fch, upg_opn);
        
        cpi_changed = true;     cookies_changed = true;     cps_changed = true;             // Set flags
    }

    // Check if Building B/S viable
    if (cookies_changed || mode_changed || varrow_changed) {
        bld_isviable = (gs_p->mode == BUY && GE_BigNum(&gs_p->cookies, bld_cost_p)) || (gs_p->mode == SELL && *bld_num_p > 0);
        if (bld_isviable != bld_wasviable) varrow_changed = true;   bld_wasviable = bld_isviable;
    }

    // Check if Upgrade B/S viable
    if (cookies_changed || mode_changed || harrow_changed) {
        upg_isviable = (gs_p->mode == BUY && *upg_num_p > 0 && GE_BigNum(&gs_p->cookies, upg_cost_p)) || (gs_p->mode == SELL && *lvl_p > 0);
        // if (upg_isviable != upg_wasviable) upgradability_change = true;
        // upg_wasviable = upg_isviable;
    }

    // Play Upgrade availability music
    if (cursor_lvl_fch && GE_BigNum(&gs_p->cookies, &gs_p->cursor_upg_cost)) {  cursor_lvl_fch = false;     Buzzer_Play(&cursor_note);
    }
    if (grandma_lvl_fch && GE_BigNum(&gs_p->cookies, &gs_p->grandma_upg_cost)) {    grandma_lvl_fch = false;    Buzzer_Play(&grandma_note);
    }
    
    // Print Upgrade description text
    if (cookies_changed || mode_changed || harrow_changed) PrintUpgDesc(hal_p, gs_p, upg_opn, lvl_p, upg_isviable);

    // Draw Vertical (Bulding selector) arrow
    if (varrow_changed) DrawVarrow(hal_p, bld_opn, bld_isviable);

    // Draw Horizontal (Upgrade selector) arrows
    if (harrow_changed) DrawHarrows(hal_p, upg_opn);

    // Print current upgrade cost
    if (harrow_changed || cpi_changed) PrintUpgCost(hal_p, upg_cost_p);

    // Print the other stats
    if (cookies_changed) PrintCookies(hal_p, gs_p);
    
    if (mode_changed) PrintMode(hal_p, gs_p);
    
    if (cps_changed) PrintCPS(hal_p, gs_p);

    if (cursor_changed) PrintCursor(hal_p, gs_p);

    if (grandma_changed) PrintGrandma(hal_p, gs_p);

}

void dummyend();
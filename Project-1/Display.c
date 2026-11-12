#include <Display.h>

uint32_t CircularIncrement(uint32_t value, uint32_t maximum) {
  return (value + 1) % maximum;
}

unsigned chars_to_uint(const char *inp, uint8_t inp_size, uint8_t count) {
    unsigned result = 0;
    uint8_t i;

    for (i = 0; i < count && i < inp_size; i++) {
        result = result * 10 + (inp[i] - '0');
    }

    return result;
}

void UART_printMessage(HAL* hal_p, const char* msg) {
  while (*msg != '\0') {
    if (UART_canSend(&hal_p->uart)) {
        UART_sendChar(&hal_p->uart, *msg);
        msg++;
    }
  }
}

uint8_t get_square_idx(uint8_t idx, uint8_t grid_length) {
    return idx > SQR(grid_length) / 2 ? idx : idx - 1;
}

// This function initializes the graphics
void InitGraphics(Graphics_Context* g_sContext_p) {

    Graphics_initContext(g_sContext_p,
                         &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_ORANGE);
    Graphics_setBackgroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(g_sContext_p, &g_sFontCm12);

    InitFonts();

    Graphics_clearDisplay(g_sContext_p);
}

// This function initializes the fonts we can use
void InitFonts() {
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
}

void Initialize(Graphics_Context* g_sContext_p) {
    InitGraphics(g_sContext_p);
}

// This function updates the foreground color of the graphic context based on the "color" argument
void change_pen_color(Graphics_Context* g_sContext_p, color_t color) {
    switch(color) {
    case BLACK:
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
        break;
    case WHITE:
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
        break;
    case RED:
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_RED);
        break;
    case ORANGE:
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_ORANGE);
        break;
    case YELLOW:
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_YELLOW);
        break;
    case BLUE:
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
        break;
    case GREEN:
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_GREEN);
        break;
    }
}

// Draws square on LCD
void draw_territory_square(Graphics_Context* g_sContext_p, display_t* disp_p, square_t* square) {
    change_pen_color(g_sContext_p, square->color);

    Graphics_fillRectangle(g_sContext_p, &square->R); // Draw the territory square
    
}

// Generates square's info
void gen_square(uint8_t grid_length, square_t* square) {
    square->R.xMin = GRID_X_START + (GRID_X_RANGE / grid_length) * square->col;
    square->R.xMax = square->R.xMin + (GRID_X_RANGE / grid_length) - SQUARE_SPACING;
    square->R.yMin = GRID_Y_START + (GRID_Y_RANGE / grid_length) * square->row;
    square->R.yMax = square->R.yMin + (GRID_Y_RANGE / grid_length) - SQUARE_SPACING;
}


// Handles flood
void Flood(HAL* hal_p, game_state_t* game_state_p) {
    static unsigned penalty = 0;

    if (game_state_p->new_cmd || game_state_p->cmd.action == INVALID) {
        penalty = 5 * (game_state_p->year / 5);

        UART_printMessage(hal_p, "\r\n\r\n** The River Swells! **\r\n");
        UART_printMessage(hal_p, "Floods threaten to destroy the land.\r\n");
        
        char Line[100];
        snprintf(Line, sizeof(Line), "Options: SP (-%d TR), AP (-%d PE), IG.\r\n", 10 + penalty, 10 + penalty);
        UART_printMessage(hal_p, Line);

        snprintf(Line, sizeof(Line), "Ignoring (IG) will cause -%d TR and -%d PE.\r\n", 5 + penalty, 5 + penalty); 
        UART_printMessage(hal_p, Line);

        UART_printMessage(hal_p, "What will you do? (SP, AP, or IG)\r\n");
        UART_printMessage(hal_p, "Enter command: ");

        game_state_p->cmd.action = NONE;
        game_state_p->new_cmd = false;
    }


    if (game_state_p->cmd.action != INVALID && game_state_p->cmd.action != NONE) {
        switch (game_state_p->cmd.action)
        {
            case IG:
                game_state_p->PE -= 5 + penalty;
                game_state_p->TR -= 5 + penalty;
                
                break;

            case SP:
                // game_state_p->PE -= 5;
                game_state_p->TR -= 10 + penalty;
                break;
            
            case AP:
                game_state_p->PE -= 10 + penalty;
                // game_state_p->TR -= 5;
                break;

            default:
                break;
        }
        game_state_p->stats_change = true;
        game_state_p->new_cmd = true;
        game_state_p->cmd.action = NONE;
    }

}

// Handles famine
void Famine(HAL* hal_p, game_state_t* game_state_p) {
    static unsigned penalty = 0;

    if (game_state_p->new_cmd || game_state_p->cmd.action == INVALID) {
        penalty = 5 * (game_state_p->year / 5);

        UART_printMessage(hal_p, "\r\n\r\n** Famine Looms! **\r\n");
        UART_printMessage(hal_p, "A harsh winter threatens the kingdom.\r\n");

        char Line[100];
        snprintf(Line, sizeof(Line), "Options: SP (-%d TR), AP (-%d PE), IG.\r\n", 15 + penalty, 15 + penalty);
        UART_printMessage(hal_p, Line);

        snprintf(Line, sizeof(Line), "Ignoring (IG) will cause -%d TR and -%d PE.\r\n", 10 + penalty, 10 + penalty);
        UART_printMessage(hal_p, Line);

        UART_printMessage(hal_p, "What will you do? (SP, AP, or IG)\r\n");
        UART_printMessage(hal_p, "Enter command: ");

        game_state_p->cmd.action = NONE;
        game_state_p->new_cmd = false;
    }


    if (game_state_p->cmd.action != INVALID && game_state_p->cmd.action != NONE) {
        switch (game_state_p->cmd.action)
        {
            case IG:
                game_state_p->PE -= 10 + penalty;
                game_state_p->TR -= 10 + penalty;
                break;

            case SP:
                game_state_p->TR -= 15 + penalty;
                break;
            
            case AP:
                game_state_p->PE -= 15 + penalty;
                break;
            default:
                break;
            
        }

        game_state_p->stats_change = true;
        game_state_p->new_cmd = true;
        game_state_p->cmd.action = NONE;
    }
}

// Handles Raid event
void Raid(Graphics_Context* g_sContext_p, display_t* disp_p, HAL* hal_p, game_state_t* game_state_p) {
    static uint8_t raid_region;
    uint8_t sq_idx;
    static unsigned penalty = 0;


    if (game_state_p->new_cmd || game_state_p->cmd.action == INVALID) {
        penalty = 5 * (game_state_p->year / 5);

        if (game_state_p->new_cmd)
            do { 
                raid_region = 1 + rand() % (SQR(game_state_p->grid_length) - 1);
            } while (game_state_p->squares[get_square_idx(raid_region, game_state_p->grid_length)].color != GREEN);

        UART_printMessage(hal_p, "\r\n\r\n** Raiders at the Gates! **\r\n");

        char Line[100];
        snprintf(Line, sizeof(Line), "They are threatening region %d!\r\n", raid_region);
        UART_printMessage(hal_p, Line);

        snprintf(Line, sizeof(Line), "Options: SP (-%d TR), AP (-%d PE), IG.\r\n", 20 + penalty, 15 + penalty);
        UART_printMessage(hal_p, Line);

        UART_printMessage(hal_p, "Ignoring (IG) will cause the region to fall.\r\n");
        UART_printMessage(hal_p, "What will you do? (SP, AP, or IG)\r\n");
        UART_printMessage(hal_p, "Enter command: ");

        game_state_p->cmd.action = NONE;
        game_state_p->new_cmd = false;
    }


    if (game_state_p->cmd.action != INVALID && game_state_p->cmd.action != NONE) {
        switch (game_state_p->cmd.action)
        {
            case IG:
                sq_idx = get_square_idx(raid_region, game_state_p->grid_length);

                game_state_p->squares[sq_idx].color = RED;
                draw_territory_square(g_sContext_p, disp_p, &game_state_p->squares[sq_idx]);
                game_state_p->num_green--;      // One less green square now

                break;

            case SP:
                game_state_p->TR -= 20 + penalty;
                break;
            
            case AP:
                game_state_p->PE -= 15 + penalty;
                break;
            default:
                break;
        }

        if (game_state_p->PE >= 0 && game_state_p->TR >= 0) game_state_p->year++;
        
        game_state_p->stats_change = true;
        game_state_p->new_cmd = true;
        game_state_p->cmd.action = NONE;
    }
    
}


// Handles Calm event
void Calm(Graphics_Context* g_sContext_p, display_t* disp_p, HAL* hal_p, game_state_t* game_state_p) {
    static unsigned penalty = 0;
    
    uint8_t max_idx;

    if (game_state_p->new_cmd || game_state_p->cmd.action == INVALID) {
        penalty = 5 * (game_state_p->year / 5);
        
        max_idx = SQR(game_state_p->grid_length) - 1;
        
        UART_printMessage(hal_p, "\r\n\r\n** Calm Period **\r\n");
        UART_printMessage(hal_p, "Your kingdom is thriving.\r\n");

        char Line[100];
        snprintf(Line, sizeof(Line), "Choose a region to rebuild (1-%d).\r\n", max_idx);
        UART_printMessage(hal_p, Line);

        UART_printMessage(hal_p, "Regions are numbered left-to-right starting from 1, row-wise (excluding center).\r\n");

        snprintf(Line, sizeof(Line), "Format: SP[1-%d] or AP[1-%d], Example: SP2.\r\n", max_idx, max_idx);
        UART_printMessage(hal_p, Line);

        snprintf(Line, sizeof(Line), "Options: SP[1-%d] (-%d TR), AP[1-%d] (-%d PE), IG.\r\n", max_idx, 30 + penalty, max_idx, 30 + penalty);
        UART_printMessage(hal_p, Line);

        UART_printMessage(hal_p, "Ignoring (IG) will cause +40 TR and +20 PE.\r\n");
        UART_printMessage(hal_p, "Enter command: ");

        game_state_p->cmd.action = NONE;
        game_state_p->new_cmd = false;
    }


    if (game_state_p->cmd.action != INVALID && game_state_p->cmd.action != NONE) {
        switch (game_state_p->cmd.action)
        {
            case IG:
                game_state_p->PE += 20;
                game_state_p->TR += 40;
                
                break;

            case SP:            
            case AP:
                if (game_state_p->cmd.action == SP) game_state_p->TR -= 30 + penalty;
                else                                game_state_p->PE -= 30 + penalty;
                
                game_state_p->squares[game_state_p->cmd.sq_idx].color = GREEN;
                draw_territory_square(g_sContext_p, disp_p, &game_state_p->squares[game_state_p->cmd.sq_idx]);
                game_state_p->num_green++;      // One more green square now

                break;
            default:
                break;
        }

        game_state_p->stats_change = true;
        game_state_p->new_cmd = true;
        game_state_p->cmd.action = NONE;
    }
    
}

void MainScreen_Text(Graphics_Context* g_sContext_p, uint8_t* j) {
    Graphics_clearDisplay(g_sContext_p);
    change_pen_color(g_sContext_p, WHITE);   // Text color
    
    uint8_t i = 0;
    // Game's name
    Graphics_drawString(g_sContext_p, (int8_t *)"Pixel Monarch", -1, 10, MIN_LINE_SPACING * ++i, true);

    // Developer's name
    Graphics_drawString(g_sContext_p, (int8_t *)"Subhan Bihan", -1, 5, MIN_LINE_SPACING * ++i, true);
    *j = i;

    // Options
    Graphics_drawString(g_sContext_p, (int8_t *)"  Play Game", -1, 5, MIN_LINE_SPACING * ++i, true);
    Graphics_drawString(g_sContext_p, (int8_t *)"  Instructions", -1, 5, MIN_LINE_SPACING * ++i, true);
    Graphics_drawString(g_sContext_p, (int8_t *)"  High Scores", -1, 5, MIN_LINE_SPACING * ++i, true);

    // Basic navigation
    Graphics_drawString(g_sContext_p, (int8_t *)"BB1: Select Item", -1, 1, MIN_LINE_SPACING * ++i, true);
    Graphics_drawString(g_sContext_p, (int8_t *)"BB2: Move Cursor", -1, 1, MIN_LINE_SPACING * ++i, true);
    Graphics_drawString(g_sContext_p, (int8_t *)"     Down", -1, 1, MIN_LINE_SPACING * ++i, true);
}

void MainScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p) {
    static option_t option_num;
    static uint8_t j;

    if (disp_p->screen_changed) {
        MainScreen_Text(g_sContext_p, &j);

        disp_p->screen_changed = false;
        disp_p->redraw = true;
        option_num = OPTION_ONE;
    }

    if (Button_isTapped(&hal_p->boosterpackS2)) {
        option_num = CircularIncrement(option_num, NUM_OPTIONS);
        disp_p->redraw = true;
    }


    if (disp_p->redraw) {
        change_pen_color(g_sContext_p, BLUE);
        
        switch (option_num) {
            case OPTION_ONE:
            default:
                Graphics_drawString(g_sContext_p, (int8_t *)">", -1, 5, MIN_LINE_SPACING * (j + 1), true);
                Graphics_drawString(g_sContext_p, (int8_t *)"  ", -1, 5, MIN_LINE_SPACING * (j + 2), true);
                Graphics_drawString(g_sContext_p, (int8_t *)"  ", -1, 5, MIN_LINE_SPACING * (j + 3), true);
                break;
            case OPTION_TWO:
                Graphics_drawString(g_sContext_p, (int8_t *)"  ", -1, 5, MIN_LINE_SPACING * (j + 1), true);
                Graphics_drawString(g_sContext_p, (int8_t *)">", -1, 5, MIN_LINE_SPACING * (j + 2), true);
                Graphics_drawString(g_sContext_p, (int8_t *)"  ", -1, 5, MIN_LINE_SPACING * (j + 3), true);
                break;
            case OPTION_THREE:
                Graphics_drawString(g_sContext_p, (int8_t *)"  ", -1, 5, MIN_LINE_SPACING * (j + 1), true);
                Graphics_drawString(g_sContext_p, (int8_t *)"  ", -1, 5, MIN_LINE_SPACING * (j + 2), true);
                Graphics_drawString(g_sContext_p, (int8_t *)">", -1, 5, MIN_LINE_SPACING * (j + 3), true);
                break;
        }

        disp_p->redraw = false;
    }


    if (Button_isTapped(&hal_p->boosterpackS1)) {
        switch (option_num)
        {
            case OPTION_ONE:
                disp_p->screen = GRIDSELECT;
                break;
            case OPTION_TWO:
                disp_p->screen = INSTRUCTIONS;
                break;
            case OPTION_THREE:
                disp_p->screen = HS;
                break;
        }
        disp_p->screen_changed = true;
    }

}


void InstructionsScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p) {
    if (disp_p->screen_changed) {
        Graphics_clearDisplay(g_sContext_p);
        change_pen_color(g_sContext_p, WHITE);   // Text color

        int8_t i = -1;

        // Constant fluff-text
        Graphics_drawString(g_sContext_p, (int8_t *)"Game Instructions", -1, 5, MIN_LINE_SPACING * ++i, true);

        Graphics_drawString(g_sContext_p, (int8_t *)"Balance People (PE),", -1, 1, MIN_LINE_SPACING * ++i, true);
        Graphics_drawString(g_sContext_p, (int8_t *)"Treasury (TR), and", -1, 1, MIN_LINE_SPACING * ++i, true);
        Graphics_drawString(g_sContext_p, (int8_t *)"territory (boxes).", -1, 1, MIN_LINE_SPACING * ++i, true);

        Graphics_drawString(g_sContext_p, (int8_t *)"When a crisis hits,", -1, 1, MIN_LINE_SPACING * ++i, true);
        Graphics_drawString(g_sContext_p, (int8_t *)"type a command:", -1, 1, MIN_LINE_SPACING * ++i, true);
        Graphics_drawString(g_sContext_p, (int8_t *)"SP: Spend (uses TR)", -1, 1, MIN_LINE_SPACING * ++i, true);
        Graphics_drawString(g_sContext_p, (int8_t *)"AP: Appeal (uses PE)", -1, 1, MIN_LINE_SPACING * ++i, true);
        Graphics_drawString(g_sContext_p, (int8_t *)"IG: Ignore", -1, 1, MIN_LINE_SPACING * ++i, true);

        Graphics_drawString(g_sContext_p, (int8_t *)"BB2: Go Back", -1, 1, 5 + MIN_LINE_SPACING * ++i, true);

        disp_p->screen_changed = false;
    }

    if (Button_isTapped(&hal_p->boosterpackS2)) {
        disp_p->screen = MAIN;
        disp_p->screen_changed = true;
    }
}

void HSScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p, game_state_t* game_state_p) {
    if (disp_p->screen_changed) {
        Graphics_clearDisplay(g_sContext_p);
        change_pen_color(g_sContext_p, WHITE);   // Text color

        uint8_t i, j = 1;

        Graphics_drawString(g_sContext_p, (int8_t *)"High Scores", -1, 30, MAX_LINE_SPACING * j, true);

        // Display the scores
        char score_string[18];
        for (i = 0; i < NUM_SCORES; i++) {
            snprintf(score_string, sizeof(score_string), "%d. %d year(s)", i + 1, game_state_p->high_scores[i]);
            Graphics_drawString(g_sContext_p, score_string, -1, 30, MIN_LINE_SPACING * (i + 1) + MAX_LINE_SPACING * j, true);
        }

        Graphics_drawString(g_sContext_p, (int8_t *)"BB2: Go Back", -1, 10, MIN_LINE_SPACING * (i + 1) + MAX_LINE_SPACING * j, true);

        disp_p->screen_changed = false;
    }

    if (Button_isTapped(&hal_p->boosterpackS2)) {
        disp_p->screen = MAIN;
        disp_p->screen_changed = true;
    }
}

void GridSelectScreen_Text(Graphics_Context* g_sContext_p, uint8_t* j) {
    Graphics_clearDisplay(g_sContext_p);
    change_pen_color(g_sContext_p, WHITE);   // Text color

    uint8_t i = 1;

    Graphics_drawString(g_sContext_p, (int8_t *)"Select Grid Size", -1, 10, MIN_LINE_SPACING * ++i, true);

    *j = i;

    Graphics_drawString(g_sContext_p, (int8_t *)"  3x3", -1, 5, MIN_LINE_SPACING * ++i, true);
    Graphics_drawString(g_sContext_p, (int8_t *)"  5x5", -1, 5, MIN_LINE_SPACING * ++i, true);

    // Basic navigation
    Graphics_drawString(g_sContext_p, (int8_t *)"BB1: Select Item", -1, 1, MIN_LINE_SPACING * ++i, true);
    Graphics_drawString(g_sContext_p, (int8_t *)"BB2: Move Cursor", -1, 1, MIN_LINE_SPACING * ++i, true);
    Graphics_drawString(g_sContext_p, (int8_t *)"     Down", -1, 1, MIN_LINE_SPACING * ++i, true);
}

void GridSelectScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p, game_state_t* game_state_p) {
    static option_t option_num;

    static uint8_t j;

    char Line[30];

    if (disp_p->screen_changed) {
        GridSelectScreen_Text(g_sContext_p, &j);

        disp_p->screen_changed = false;
        disp_p->redraw = true;
        option_num = OPTION_ONE;
        game_state_p->defstats = ASK_PE;
    }

    if (Button_isTapped(&hal_p->boosterpackS2)) {
        option_num = CircularIncrement(option_num, OPTION_THREE);
        disp_p->redraw = true;
    }

    // char op1[50], op2[50], op3[50];

    if (disp_p->redraw) {
        change_pen_color(g_sContext_p, BLUE);
        
        switch (option_num) {
            case OPTION_ONE:
            default:
                Graphics_drawString(g_sContext_p, (int8_t *)">", -1, 5, MIN_LINE_SPACING * (j + 1), true);
                Graphics_drawString(g_sContext_p, (int8_t *)"  ", -1, 5, MIN_LINE_SPACING * (j + 2), true);
                break;
            case OPTION_TWO:
                Graphics_drawString(g_sContext_p, (int8_t *)"  ", -1, 5, MIN_LINE_SPACING * (j + 1), true);
                Graphics_drawString(g_sContext_p, (int8_t *)">", -1, 5, MIN_LINE_SPACING * (j + 2), true);
                break;
        }

        disp_p->redraw = false;
    }

    if (game_state_p->defstats != STATS_DONE)
        switch (game_state_p->defstats) {
            case ASK_PE:
                snprintf(Line, sizeof(Line), "\r\nInitial PE [0 - %d]: ", MAX_INIT_STATS);
                UART_printMessage(hal_p, Line);
                game_state_p->defstats = INP_PE;
                break;
            case ASK_TR:
                snprintf(Line, sizeof(Line), "\r\nInitial TR [0 - %d]: ", MAX_INIT_STATS);
                UART_printMessage(hal_p, Line);
                game_state_p->defstats = INP_TR;
                break;
            default:
                break;
        }

    if (Button_isTapped(&hal_p->boosterpackS1)) {
        switch (option_num)
        {
            case OPTION_ONE:
                game_state_p->grid_length = 3;
                break;
            case OPTION_TWO:
                game_state_p->grid_length = 5;
                break;
        }

        disp_p->screen = GAME;
        disp_p->screen_changed = true;
    }
}

void GameScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p, game_state_t* game_state_p, uint32_t counter) {
    char PE_str[30], TR_str[30], year_str[30];
    static const color_t colors[] = {GREEN, RED};
    static const unsigned colors_size = sizeof(colors) / sizeof(color_t);

    if (disp_p->screen_changed) {
        Graphics_clearDisplay(g_sContext_p);

        game_state_p->PE = game_state_p->Default_PE;
        game_state_p->TR = game_state_p->Default_TR;
        game_state_p->year = 1;
        game_state_p->num_green = 0;

        srand(counter);

        // Set (row, col) for each square
        uint8_t i;
        for (i = 0; i < SQR(game_state_p->grid_length); i++) {
            game_state_p->squares[i].row = i / game_state_p->grid_length;
            game_state_p->squares[i].col = i % game_state_p->grid_length;
            game_state_p->squares[i].color = colors[rand() % colors_size];
            game_state_p->num_green += (game_state_p->squares[i].color == GREEN);   // Count the number of green squares
            gen_square(game_state_p->grid_length, &game_state_p->squares[i]);
        }

        // Adjust the center square
        unsigned cnt_idx = SQR(game_state_p->grid_length) / 2;
        game_state_p->squares[cnt_idx].color = WHITE;
        game_state_p->squares[cnt_idx].R.xMin += 5;
        game_state_p->squares[cnt_idx].R.yMin += 5;
        game_state_p->squares[cnt_idx].R.xMax -= 5;
        game_state_p->squares[cnt_idx].R.yMax -= 5;

        // Draw the squares
        for (i = 0; i < SQR(game_state_p->grid_length); i++) {
            draw_territory_square(g_sContext_p,  disp_p, &game_state_p->squares[i]);
        }

        disp_p->screen_changed = false;
        game_state_p->stats_change = true;
        game_state_p->new_cmd = true;
    }

    if (game_state_p->stats_change) {
        change_pen_color(g_sContext_p, WHITE);   // Text color
        
        // Stats text
        snprintf(PE_str, sizeof(PE_str), "PE:%4d", max(game_state_p->PE, 0));
        snprintf(TR_str, sizeof(TR_str), "TR:%4d", max(game_state_p->TR, 0));
        snprintf(year_str, sizeof(year_str), "Year:%3d", game_state_p->year);

        // Print stats
        Graphics_drawString(g_sContext_p, PE_str, -1, 5, 1, true);
        Graphics_drawString(g_sContext_p, TR_str, -1, 5, 11, true);
        Graphics_drawString(g_sContext_p, year_str, -1, 80, 6, true);

        game_state_p->stats_change = false;
    }

    // Check for loss conditions - end game if any met
    if (game_state_p->PE <= 0 || game_state_p->TR <= 0 || game_state_p->num_green == 0) {
        disp_p->screen = GAMEOVER;
        disp_p->screen_changed = true;
        return;
    }


    // Randomly select one of the four events
    if (game_state_p->new_cmd) game_state_p->event = rand() % NUM_EVENTS;

    switch (game_state_p->event)
    {
        case FLOOD:
            Flood(hal_p, game_state_p);
            break;
        case FAMINE:
            Famine(hal_p, game_state_p);
            break;
        case RAID:
            Raid(g_sContext_p, disp_p, hal_p, game_state_p);
            break;
        case CALM:
            Calm(g_sContext_p, disp_p, hal_p, game_state_p);
            break;
    }

}

void GameOverScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p, game_state_t* game_state_p) {
    if (disp_p->screen_changed) {
        Graphics_clearDisplay(g_sContext_p);
        uint8_t i = 0, j = 0;

        // Display the text
        change_pen_color(g_sContext_p, RED);   // Text color
        Graphics_drawString(g_sContext_p, (int8_t *)"Your Kingdom", -1, 10, MIN_LINE_SPACING * ++i, true);
        Graphics_drawString(g_sContext_p, (int8_t *)" Has Fallen!", -1, 10, MIN_LINE_SPACING * ++i, true);

        change_pen_color(g_sContext_p, WHITE);   // Text color
        Graphics_drawString(g_sContext_p, (int8_t *)"You Survived For:", -1, 10, MIN_LINE_SPACING * i + MAX_LINE_SPACING * ++j, true);

        char score_string[18];
        snprintf(score_string, sizeof(score_string), "%d year(s)", game_state_p->year);
        Graphics_drawString(g_sContext_p, score_string, -1, 20, MIN_LINE_SPACING * ++i + MAX_LINE_SPACING * j, true);

        Graphics_drawString(g_sContext_p, (int8_t *)"Press BB1", -1, 20, MIN_LINE_SPACING * i + MAX_LINE_SPACING * ++j, true);
        Graphics_drawString(g_sContext_p, (int8_t *)"to restart", -1, 20, MIN_LINE_SPACING * ++i + MAX_LINE_SPACING * j, true);

        // Update the high scores
        if (game_state_p->year > game_state_p->high_scores[NUM_SCORES - 1])         // No need to update if this isn't met
            for (i = 0; i < NUM_SCORES; i++) {
                if (game_state_p->year >= game_state_p->high_scores[i]) {
                    for (j = NUM_SCORES - 1; j > i; j--) {
                        game_state_p->high_scores[j] = game_state_p->high_scores[j-1];
                    }
                    game_state_p->high_scores[i] = game_state_p->year;

                    break;
                }
            }
        disp_p->screen_changed = false;
    }

    if (Button_isTapped(&hal_p->boosterpackS1)) {
        disp_p->screen = MAIN;
        disp_p->screen_changed = true;
    }
}
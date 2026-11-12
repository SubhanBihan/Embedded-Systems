#include <Display.h>

uint32_t CircularIncrement(uint32_t value, uint32_t maximum) {
  return (value + 1) % maximum;
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
        // TODO: Add more cases if needed.
    }
}

void draw_territory_square(Graphics_Context* g_sContext_p, display_t* disp_p, square_t* square) {
    
    // if (!disp_p->screen_changed) { 
    //     change_pen_color(g_sContext_p, BLACK);
    //     Graphics_fillRectangle(g_sContext_p, &square->R);
    // }

    change_pen_color(g_sContext_p, square->color);
    Graphics_fillRectangle(g_sContext_p, &square->R);
    
}

void gen_square(uint8_t grid_length, square_t* square) {
    square->R.xMin = GRID_X_START + (GRID_X_RANGE / grid_length) * square->col;
    square->R.xMax = square->R.xMin + (GRID_X_RANGE / grid_length) - SQUARE_SPACING;
    square->R.yMin = GRID_Y_START + (GRID_Y_RANGE / grid_length) * square->row;
    square->R.yMax = square->R.yMin + (GRID_Y_RANGE / grid_length) - SQUARE_SPACING;
}

void MainScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p) {
    static option_t option_num = ONE;

    static uint8_t j;

    if (disp_p->screen_changed) {
        // // wipe the screen by drawing over it with the black color as the the background
        // Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);

        // Might need to change this to only covering the '>'- DONE (?)
        Graphics_clearDisplay(g_sContext_p);
        change_pen_color(g_sContext_p, WHITE);   // Text color

        uint8_t i = 0;

        // Game's name
        Graphics_drawString(g_sContext_p, (int8_t *)"Pixel Monarch", -1, 10, MIN_LINE_SPACING * ++i, true);

        // Developer's name
        Graphics_drawString(g_sContext_p, (int8_t *)"Subhan Bihan", -1, 5, MIN_LINE_SPACING * ++i, true);
        j = i;

        // Options
        Graphics_drawString(g_sContext_p, (int8_t *)"  Play Game", -1, 5, MIN_LINE_SPACING * ++i, true);
        Graphics_drawString(g_sContext_p, (int8_t *)"  Instructions", -1, 5, MIN_LINE_SPACING * ++i, true);
        Graphics_drawString(g_sContext_p, (int8_t *)"  High Scores", -1, 5, MIN_LINE_SPACING * ++i, true);

        // Basic navigation
        Graphics_drawString(g_sContext_p, (int8_t *)"BB1: Select Item", -1, 1, MIN_LINE_SPACING * ++i, true);
        Graphics_drawString(g_sContext_p, (int8_t *)"BB2: Move Cursor", -1, 1, MIN_LINE_SPACING * ++i, true);
        Graphics_drawString(g_sContext_p, (int8_t *)"     Down", -1, 1, MIN_LINE_SPACING * ++i, true);

        disp_p->screen_changed = false;
        disp_p->redraw = true;
        option_num = ONE;
    }

    if (Button_isTapped(&hal_p->boosterpackS2)) {
        CircularIncrement(option_num, NUM_OPTIONS);
        disp_p->redraw = true;
    }

    // char op1[50], op2[50], op3[50];

    if (disp_p->redraw == true) {
        change_pen_color(g_sContext_p, BLUE);
        
        switch (option_num) {
            case ONE:
            default:
                Graphics_drawString(g_sContext_p, (int8_t *)">", -1, 5, MIN_LINE_SPACING * (j + 1), true);
                Graphics_drawString(g_sContext_p, (int8_t *)"  ", -1, 5, MIN_LINE_SPACING * (j + 2), true);
                Graphics_drawString(g_sContext_p, (int8_t *)"  ", -1, 5, MIN_LINE_SPACING * (j + 3), true);
                break;
            case TWO:
                Graphics_drawString(g_sContext_p, (int8_t *)"  ", -1, 5, MIN_LINE_SPACING * (j + 1), true);
                Graphics_drawString(g_sContext_p, (int8_t *)">", -1, 5, MIN_LINE_SPACING * (j + 2), true);
                Graphics_drawString(g_sContext_p, (int8_t *)"  ", -1, 5, MIN_LINE_SPACING * (j + 3), true);
                break;
            case THREE:
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
            case ONE:
                disp_p->screen = GAME;
                break;
            case TWO:
                disp_p->screen = INSTRUCTIONS;
                break;
            case THREE:
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

        char i = 0;

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

        Graphics_drawString(g_sContext_p, (int8_t *)"BB2: Go Back", -1, 1, MIN_LINE_SPACING * ++i, true);

        disp_p->screen_changed = false;
    }

    if (Button_isTapped(&hal_p->boosterpackS2)) {
        disp_p->screen = MAIN;
        disp_p->screen_changed = true;
    }
}

void HSScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p, unsigned current_score) {
    static unsigned int highscores[NUM_SCORES] = {0};

    if (disp_p->screen_changed) {
        Graphics_clearDisplay(g_sContext_p);
        change_pen_color(g_sContext_p, WHITE);   // Text color

        // Update the scores
        char i, j;

        if (current_score > highscores[NUM_SCORES])         // No need to update if this isn't met
            for (i = 0; i < NUM_SCORES; i++) {
                if (current_score >= highscores[i]) {
                    for (j = NUM_SCORES; j > i; j--) {
                        highscores[j] = highscores[j-1];
                    }
                    highscores[i] = current_score;

                    break;
                }
            }

        // Display the scores
        Graphics_drawString(g_sContext_p, (int8_t *)"High Scores", -1, 30, 20, true);

        char score_string[18];
        for (i = 0; i < NUM_SCORES; i++) {
            snprintf(score_string, sizeof(score_string), "%d. %d year(s)", i + 1, highscores[i]);
            Graphics_drawString(g_sContext_p, score_string, -1, 30, 30 + MIN_LINE_SPACING * i, true);
        }

        Graphics_drawString(g_sContext_p, (int8_t *)"BB2: Go Back", -1, 10, 100, true);

        disp_p->screen_changed = false;
    }

    if (Button_isTapped(&hal_p->boosterpackS2)) {
        disp_p->screen = MAIN;
        disp_p->screen_changed = true;
    }
}

void GameScreen(Graphics_Context* g_sContext_p, HAL* hal_p, display_t* disp_p, game_state_t* game_state_p) {
    char PE_str[30], TR_str[30], year_str[30];
    static unsigned seed = 0;

    if (disp_p->screen_changed) {
        Graphics_clearDisplay(g_sContext_p);

        game_state_p->PE = 50;
        game_state_p->TR = 100;
        game_state_p->year = 1;

        color_t colors[] = {GREEN, RED};
        unsigned colors_size = sizeof(colors) / sizeof(color_t);

        // TODO: Use Timer for random seed!!!
        srand(seed++);

        // Set (row, col) for each square
        uint8_t i;
        for (i = 0; i < SQR(game_state_p->grid_length); i++) {
            game_state_p->squares[i].row = i / game_state_p->grid_length;
            game_state_p->squares[i].col = i % game_state_p->grid_length;
            game_state_p->squares[i].color = colors[rand() % colors_size];
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
    }

    if (game_state_p->stats_change) {
        change_pen_color(g_sContext_p, WHITE);   // Text color
        
        // Stats text
        snprintf(PE_str, sizeof(PE_str), "PE:%4d", game_state_p->PE);
        snprintf(TR_str, sizeof(TR_str), "TR:%4d", game_state_p->TR);
        snprintf(year_str, sizeof(year_str), "Year:%3d", game_state_p->year);

        // Print stats
        Graphics_drawString(g_sContext_p, PE_str, -1, 5, 2, true);
        Graphics_drawString(g_sContext_p, TR_str, -1, 5, 12, true);
        Graphics_drawString(g_sContext_p, year_str, -1, 80, 7, true);

        game_state_p->stats_change = false;
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

    if (Button_isTapped(&hal_p->boosterpackS1)) {
        disp_p->screen = MAIN;          // TODO: Change this to GAMEOVER for project
        disp_p->screen_changed = true;
        game_state_p->stats_change = true;
        game_state_p->new_cmd = true;
    }
    
}



void Flood(HAL* hal_p, game_state_t* game_state_p) {
    // static command_t cmd = {NONE, MAX_GRID};

    if (game_state_p->new_cmd || game_state_p->cmd.action == INVALID) {
        // if (game_state_p->cmd.action == INVALID)
        //     UART_printMessage(hal_p, "\r\n\r\nInvalid Input. Try again!\r\n\r\n");

        UART_printMessage(hal_p, "\r\n\r\n** The River Swells! **\r\n");
        UART_printMessage(hal_p, "Floods threaten to destroy the land.\r\n");
        UART_printMessage(hal_p, "Options: SP (-10 TR), AP (-10 PE), IG.\r\n");       //TODO (small): How to change font color in terminal?
        UART_printMessage(hal_p, "Ignoring (IG) will cause -5 TR and -5 PE.\r\n");
        UART_printMessage(hal_p, "What will you do? (SP, AP, or IG)\r\n");
        UART_printMessage(hal_p, "Enter command: ");

        game_state_p->cmd.action = NONE;
        game_state_p->new_cmd = false;
    }


    if (game_state_p->cmd.action != INVALID && game_state_p->cmd.action != NONE) {
        switch (game_state_p->cmd.action)
        {
            case IG:
                game_state_p->PE -= 5;
                game_state_p->TR -= 5;
                
                break;

            case SP:
                // game_state_p->PE -= 5;
                game_state_p->TR -= 10;
                break;
            
            case AP:
                game_state_p->PE -= 10;
                // game_state_p->TR -= 5;
                break;

            default:
                break;
        }
        game_state_p->stats_change = true;
        game_state_p->new_cmd = true;
    }

}


void Famine(HAL* hal_p, game_state_t* game_state_p) {
    // static command_t cmd = {NONE, MAX_GRID};

    if (game_state_p->new_cmd || game_state_p->cmd.action == INVALID) {
        // if (game_state_p->cmd.action == INVALID)
        //     UART_printMessage(hal_p, "Invalid Input. Try again!\r\n\r\n");

        UART_printMessage(hal_p, "\r\n\r\n** Famine Looms! **\r\n");
        UART_printMessage(hal_p, "A harsh winter threatens the kingdom.\r\n");
        UART_printMessage(hal_p, "Options: SP (-15 TR), AP (-15 PE), IG.\r\n");
        UART_printMessage(hal_p, "Ignoring (IG) will cause -10 TR and -10 PE.\r\n");
        UART_printMessage(hal_p, "What will you do? (SP, AP, or IG)\r\n");
        UART_printMessage(hal_p, "Enter command: ");

        game_state_p->cmd.action = NONE;
        game_state_p->new_cmd = false;
    }


    if (game_state_p->cmd.action != INVALID && game_state_p->cmd.action != NONE) {
        switch (game_state_p->cmd.action)
        {
            case IG:
                game_state_p->PE -= 10;
                game_state_p->TR -= 10;
                break;

            case SP:
                // game_state_p->PE -= 5;
                game_state_p->TR -= 15;
                break;
            
            case AP:
                game_state_p->PE -= 15;
                // game_state_p->TR -= 5;
                break;
            default:
                break;
            
        }

        game_state_p->stats_change = true;
        game_state_p->new_cmd = true;
    }
}


void Raid(Graphics_Context* g_sContext_p, display_t* disp_p, HAL* hal_p, game_state_t* game_state_p) {
    // Later, need to make it so only GREEN region is attacked
    char Line[100];
    static uint8_t raid_region;
    uint8_t sq_idx;
    // static command_t cmd = {NONE, MAX_GRID};


    if (game_state_p->new_cmd || game_state_p->cmd.action == INVALID) {
        // if (game_state_p->cmd.action == INVALID)
        //     UART_printMessage(hal_p, "Invalid Input. Try again!\r\n\r\n");
        // else
        if (game_state_p->cmd.action != INVALID) 
            raid_region = 1 + rand() % (SQR(game_state_p->grid_length) - 1);

        UART_printMessage(hal_p, "\r\n\r\n** Raiders at the Gates! **\r\n");

        snprintf(Line, sizeof(Line), "They are threatening region %d!\r\n", raid_region);
        UART_printMessage(hal_p, Line);

        UART_printMessage(hal_p, "Options: SP (-20 TR), AP (-15 PE), IG.\r\n");
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
                // game_state_p->PE -= 10;
                // game_state_p->TR -= 10;
                sq_idx = get_square_idx(raid_region, game_state_p->grid_length);
                if (game_state_p->squares[sq_idx].color != RED) {
                    game_state_p->squares[sq_idx].color = RED;
                    draw_territory_square(g_sContext_p, disp_p, &game_state_p->squares[sq_idx]);
                }
                break;

            case SP:
                // game_state_p->PE -= 5;
                game_state_p->TR -= 20;
                game_state_p->stats_change = true;
                break;
            
            case AP:
                game_state_p->PE -= 15;
                // game_state_p->TR -= 5;
                game_state_p->stats_change = true;
                break;
            default:
                break;
        }

        game_state_p->year++;
        game_state_p->new_cmd = true;
    }
    
}



void Calm(Graphics_Context* g_sContext_p, display_t* disp_p, HAL* hal_p, game_state_t* game_state_p) {
    // TODO: Later, need to make it so only GREEN region is attacked

    // static command_t cmd = {NONE, MAX_GRID};
    char Line[100];
    uint8_t max_idx;

    if (game_state_p->new_cmd || game_state_p->cmd.action == INVALID) {
        // if (game_state_p->cmd.action == INVALID)
        //     UART_printMessage(hal_p, "Invalid Input. Try again!\r\n\r\n");
        
        max_idx = SQR(game_state_p->grid_length) - 1;
        
        UART_printMessage(hal_p, "\r\n\r\n** Calm Period **\r\n");
        UART_printMessage(hal_p, "Your kingdom is thriving.\r\n");

        snprintf(Line, sizeof(Line), "Choose a region to rebuild (1-%d).\r\n", max_idx);
        UART_printMessage(hal_p, Line);

        UART_printMessage(hal_p, "Regions are numbered left-to-right starting from 1, row-wise (excluding center).\r\n");

        snprintf(Line, sizeof(Line), "Format: SP[1-%d] or AP[1-%d], Example: SP2.\r\n", max_idx, max_idx);
        UART_printMessage(hal_p, Line);

        snprintf(Line, sizeof(Line), "Options: SP[1-%d] (-30 TR), AP[1-%d] (-30 PE), IG.\r\n", max_idx, max_idx);
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
                // game_state_p->PE -= 5;
                game_state_p->TR -= 30;
                if (game_state_p->squares[game_state_p->cmd.sq_idx].color != GREEN) {
                    game_state_p->squares[game_state_p->cmd.sq_idx].color = GREEN;
                    draw_territory_square(g_sContext_p, disp_p, &game_state_p->squares[game_state_p->cmd.sq_idx]);
                }
                break;
            
            case AP:
                game_state_p->PE -= 30;
                // game_state_p->TR -= 5;
                if (game_state_p->squares[game_state_p->cmd.sq_idx].color != GREEN) {
                    game_state_p->squares[game_state_p->cmd.sq_idx].color = GREEN;
                    draw_territory_square(g_sContext_p, disp_p, &game_state_p->squares[game_state_p->cmd.sq_idx]);
                }
                break;
            default:
                break;
        }

        game_state_p->stats_change = true;
        game_state_p->new_cmd = true;
    }
    
}

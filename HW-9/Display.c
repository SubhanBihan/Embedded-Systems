#include "Display.h"
// #include <Display.h>

uint32_t CircularIncrement(uint32_t value, uint32_t maximum) {
  return (value + 1) % maximum;
}

uint32_t CircularDecrement(uint32_t value, uint32_t maximum) {
    return value = (value + maximum - 1) % maximum;
}

bool IsOverlap(const Graphics_Rectangle* R1, const Graphics_Rectangle* R2) {
    return ((R1->xMax >= R2->xMin) && (R1->xMin <= R2->xMax) && (R1->yMax >= R2->yMin) && (R1->yMin <= R2->yMax));
}

void MoveRectangle(HAL* hal_p, display_t* disp_p, Graphics_Rectangle* R, motion_t* motion_p) {
    // Defines the Grid area
    static const Graphics_Rectangle Grid = {GRID_X_START, GRID_Y_START, GRID_X_END, GRID_Y_END};

    if (motion_p->exists) {
        if (motion_p->manual) {

        }
        else {
            if (motion_p->mode) {      // Continuous movement mode
                if (SWTimer_expired(&motion_p->mTimer)) {
                    // Erase the old position image
                    GFX_removeSolidSquare(&disp_p->gfx, R);

                    // Control the Horiontal and Vertical motion
                    R->xMin = R->xMin + motion_p->left_right * motion_p->dx;
                    R->xMax = R->xMax + motion_p->left_right * motion_p->dx;
                    R->yMin = R->yMin + motion_p->up_down    * motion_p->dy;
                    R->yMax = R->yMax + motion_p->up_down    * motion_p->dy;

                    // If the object needs to stay, adjust the conditions
                    if (motion_p->stay) {
                        if (R->xMin < Grid.xMin) {
                            R->xMin += motion_p->dx; R->xMax += motion_p->dx;
                            motion_p->left_right = 1;
                        }
                        else if (R->xMax > Grid.xMax) {
                            R->xMin -= motion_p->dx; R->xMax -= motion_p->dx;
                            motion_p->left_right = -1;
                        }

                        if (R->yMin < Grid.yMin) {
                            R->yMin += motion_p->dy; R->yMax += motion_p->dy;
                            motion_p->up_down = 1;
                        }
                        else if (R->yMax > Grid.yMax) {
                            R->yMin -= motion_p->dy; R->yMax -= motion_p->dy;
                            motion_p->up_down = -1;
                        }
                    }


                    if (IsOverlap(R, &Grid))           
                        // Draw the new position image
                        GFX_drawSolidSquare(&disp_p->gfx, R); 
                    else
                        // If the object is beyond the grid region, it no longer exists
                        motion_p->exists = false;

                    SWTimer_start(&motion_p->mTimer);   // Restart the timer
                }
            }
        }
    }
}


void TitleScreen(HAL* hal_p, display_t* disp_p) {
    static SWTimer TitleTimer;  // Timer for Title screen

    if (disp_p->screen_changed) {   // Only do these when first entering title screen (function)
        // Clear the display
        GFX_clear(&disp_p->gfx);
        
        // Print Game name
        GFX_print(&disp_p->gfx, "Touhou", 1, 1);

        // Print Dev's name
        GFX_print(&disp_p->gfx, "Subhan Bihan", 3, 1);

        // Construxt and start the timer
        TitleTimer = SWTimer_construct(TITLE_DURATION_MS);
        SWTimer_start(&TitleTimer);

        // Turn off the screen_changed flag
        disp_p->screen_changed = false;
    }

    // When the timer expries, change settings to move to Main Menu screen
    if (SWTimer_expired(&TitleTimer)) {
        disp_p->screen = MAIN;
        disp_p->screen_changed = true;
    }
}

// void MainScreen(HAL* hal_p, display_t* disp_p) {
//     static option_t current_option = OPTION_ONE;
//     static option_t prev_option = OPTION_ONE;
//     static uint8_t j;               // Tracking row of first option
//     uint8_t i;

//     if (disp_p->screen_changed) {   // Only do these when first entering main screen (function)
//         // MainScreen_Text(&disp_p->gfx.context, &j);

//         // Options text
//         char* options[] = {"  Play", "  Instructions", "  High Scores"};
//         uint8_t num_options = sizeof(options) / sizeof(char*);
//         char* navg_lines[] = {"BB1: Cursor Up", "BB2: Cursor Down"};

//         // Clear the display
//         GFX_clear(&disp_p->gfx);

//         // Set foreground color
//         GFX_setForeground(&disp_p->gfx, TEXT_COLOR);

//         i = 1;  // Tracking row of current line

//         // Print Screen name
//         GFX_print(&disp_p->gfx, "MENU", i, 1);
//         i += MIN_SECTION_GAP_ROWS;

//         // Print options
//         j = i;
//         i = GFX_printTextRows(&disp_p->gfx, options, num_options, j, 1);

//         // Print navigations instructions
//         i += MIN_SECTION_GAP_ROWS;
//         GFX_printTextRows(&disp_p->gfx, navg_lines, sizeof(navg_lines) / sizeof(char*), i, 1);

//         GFX_setForeground(&disp_p->gfx, ARROW_COLOR);
        
//         // Set flags to stay on this screen
//         disp_p->screen_changed = false;
//         disp_p->redraw = true;
//         current_option = OPTION_ONE;
//     }

//     if (Button_isTapped(&hal_p->boosterpackS1)) {
//         prev_option = current_option;
//         current_option = CircularDecrement(current_option, NUM_OPTIONS);
//         disp_p->redraw = true;
//     }

//     if (Button_isTapped(&hal_p->boosterpackS2)) {
//         prev_option = current_option;
//         current_option = CircularIncrement(current_option, NUM_OPTIONS);
//         disp_p->redraw = true;
//     }


//     if (disp_p->redraw) {
//         // Erase any precious cursor
//         // GFX_setForeground(&disp_p->gfx, disp_p->gfx.background);
        
//         // char* cursor_eraser[] = {" ", " ", " "};
//         // GFX_printTextRows(&disp_p->gfx, cursor_eraser, sizeof(cursor_eraser) / sizeof(char*), j, 1);
//         GFX_print(&disp_p->gfx, "  ", j + prev_option, 1);
        
//         // Print new cursor
//         // GFX_setForeground(&disp_p->gfx, ARROW_COLOR);
//         GFX_print(&disp_p->gfx, ">", j + current_option, 1);

//         disp_p->redraw = false;
//     }


//     if (Button_isTapped(&hal_p->boosterpackJS)) {
//         switch (current_option)
//         {
//             case OPTION_ONE:
//                 disp_p->screen = GAME;
//                 break;
//             case OPTION_TWO:
//                 disp_p->screen = INSTRUCTIONS;
//                 break;
//             case OPTION_THREE:
//                 disp_p->screen = HS;
//                 break;
//         }
//         disp_p->screen_changed = true;
//     }

// }

void MainScreen_Text(display_t* disp_p, uint8_t* j) {
    Graphics_clearDisplay(&disp_p->gfx.context);
    GFX_setForeground(&disp_p->gfx, TEXT_COLOR);   // Text color
    
    uint8_t i = 0;

    Graphics_drawString(&disp_p->gfx.context, (int8_t *)"MENU", -1, 5, MIN_LINE_SPACING * ++i, true);

    *j = i;

    // Options
    Graphics_drawString(&disp_p->gfx.context, (int8_t *)"  Play Game", -1, 5, MIN_LINE_SPACING * ++i, true);
    Graphics_drawString(&disp_p->gfx.context, (int8_t *)"  Instructions", -1, 5, MIN_LINE_SPACING * ++i, true);
    Graphics_drawString(&disp_p->gfx.context, (int8_t *)"  High Scores", -1, 5, MIN_LINE_SPACING * ++i, true);

    // Basic navigation
    Graphics_drawString(&disp_p->gfx.context, (int8_t *)"BB1: Cursor Up", -1, 1, MIN_LINE_SPACING * ++i, true);
    Graphics_drawString(&disp_p->gfx.context, (int8_t *)"BB2: Cursor Down", -1, 1, MIN_LINE_SPACING * ++i, true);
    Graphics_drawString(&disp_p->gfx.context, (int8_t *)"JSB: Select", -1, 1, MIN_LINE_SPACING * ++i, true);
}

void MainScreen(HAL* hal_p, display_t* disp_p) {
    static option_t option_num;
    static uint8_t j;

    if (disp_p->screen_changed) {
        MainScreen_Text(&disp_p->gfx.context, &j);

        disp_p->screen_changed = false;
        disp_p->redraw = true;
        option_num = OPTION_ONE;
    }

    if (Button_isTapped(&hal_p->boosterpackS1)) {
        // prev_option = option_num;
        option_num = CircularDecrement(option_num, NUM_OPTIONS);
        disp_p->redraw = true;
    }

    if (Button_isTapped(&hal_p->boosterpackS2)) {
        // prev_option = option_num;
        option_num = CircularIncrement(option_num, NUM_OPTIONS);
        disp_p->redraw = true;
    }


    if (disp_p->redraw) {
        GFX_setForeground(&disp_p->gfx, ARROW_COLOR);
        
        switch (option_num) {
            case OPTION_ONE:
            default:
                Graphics_drawString(&disp_p->gfx.context, (int8_t *)">", -1, 5, MIN_LINE_SPACING * (j + 1), true);
                Graphics_drawString(&disp_p->gfx.context, (int8_t *)"  ", -1, 5, MIN_LINE_SPACING * (j + 2), true);
                Graphics_drawString(&disp_p->gfx.context, (int8_t *)"  ", -1, 5, MIN_LINE_SPACING * (j + 3), true);
                break;
            case OPTION_TWO:
                Graphics_drawString(&disp_p->gfx.context, (int8_t *)"  ", -1, 5, MIN_LINE_SPACING * (j + 1), true);
                Graphics_drawString(&disp_p->gfx.context, (int8_t *)">", -1, 5, MIN_LINE_SPACING * (j + 2), true);
                Graphics_drawString(&disp_p->gfx.context, (int8_t *)"  ", -1, 5, MIN_LINE_SPACING * (j + 3), true);
                break;
            case OPTION_THREE:
                Graphics_drawString(&disp_p->gfx.context, (int8_t *)"  ", -1, 5, MIN_LINE_SPACING * (j + 1), true);
                Graphics_drawString(&disp_p->gfx.context, (int8_t *)"  ", -1, 5, MIN_LINE_SPACING * (j + 2), true);
                Graphics_drawString(&disp_p->gfx.context, (int8_t *)">", -1, 5, MIN_LINE_SPACING * (j + 3), true);
                break;
        }

        disp_p->redraw = false;
    }


    if (Button_isTapped(&hal_p->boosterpackJS)) {
        switch (option_num)
        {
            case OPTION_ONE:
                disp_p->screen = GAME;
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

void InstructionsScreen(HAL* hal_p, display_t* disp_p) {
    if (disp_p->screen_changed) {   // Only do these when first entering main screen (function)
        // MainScreen_Text(&disp_p->gfx.context, &j);

        // // Instructions text
        // char* lines[] = {"  Play", "  Instructions", "  High Scores"};
        // uint8_t num_lines = sizeof(lines) / sizeof(char*);

        // Clear the display
        GFX_clear(&disp_p->gfx);

        // Set foreground color
        GFX_setForeground(&disp_p->gfx, TEXT_COLOR);

        uint8_t i = 1;  // Tracking row of current line

        // Print Screen name
        GFX_print(&disp_p->gfx, "INSTRUCTIONS", i++, 1);
        i += MIN_SECTION_GAP_ROWS;

        // Print options
        // GFX_printTextRows(&disp_p->gfx, lines, num_lines, i, 1);

        // Print navigations instructions

        // Set flags to stay on this screen
        disp_p->screen_changed = false;
    }

    if (Button_isTapped(&hal_p->boosterpackS2)) {
        disp_p->screen = MAIN;
        disp_p->screen_changed = true;
    }
}

void HSScreen(HAL* hal_p, display_t* disp_p) {
    if (disp_p->screen_changed) {   // Only do these when first entering main screen (function)
        // MainScreen_Text(&disp_p->gfx.context, &j);

        // // Instructions text
        // char* lines[] = {"  Play", "  Instructions", "  High Scores"};
        // uint8_t num_lines = sizeof(lines) / sizeof(char*);

        // Clear the display
        GFX_clear(&disp_p->gfx);

        // Set foreground color
        GFX_setForeground(&disp_p->gfx, TEXT_COLOR);

        uint8_t i = 1;  // Tracking row of current line

        // Print Screen name
        GFX_print(&disp_p->gfx, "HIGH SCORES", i++, 1);
        i += MIN_SECTION_GAP_ROWS;

        // Print options
        // GFX_printTextRows(&disp_p->gfx, lines, num_lines, i, 1);

        // Print navigations instructions

        // Set flags to stay on this screen
        disp_p->screen_changed = false;
    }

    if (Button_isTapped(&hal_p->boosterpackS2)) {
        disp_p->screen = MAIN;
        disp_p->screen_changed = true;
    }
}


void GameScreen(HAL* hal_p, display_t* disp_p, game_state_t* game_state_p) {
    char line[20];

    if (disp_p->screen_changed) {   // Only do these when first entering main screen (function)
        // Clear the display
        GFX_clear(&disp_p->gfx);

        // Set foreground color
        GFX_setForeground(&disp_p->gfx, TEXT_COLOR);

        uint8_t i = 0;  // Tracking row of current line

        // Print Screen name
        GFX_print(&disp_p->gfx, "GAME", i++, 0);

        // Set foreground color
        GFX_setForeground(&disp_p->gfx, PLAYER_COLOR);

        // Draw Player HP circle (HPC)
        GFX_drawSolidCircle(&disp_p->gfx, PLAYER_HPC_X, PLAYER_HPC_Y, HPC_RADIUS);

        // Start the player's motion timer
        SWTimer_start(&game_state_p->player.motion.mTimer);

        // Set flags to stay on this screen
        disp_p->screen_changed = false;
        disp_p->redraw = true;
    }


    if (disp_p->redraw) {
        // Set foreground color
        GFX_setForeground(&disp_p->gfx, TEXT_COLOR);

        // Player HP Text
        snprintf(line, sizeof(line), "%d", game_state_p->player.HP);
        // GFX_print(&disp_p->gfx, line, PLAYER_HPTEXT_X, PLAYER_HPTEXT_Y);
        Graphics_drawString(&disp_p->gfx.context, line, -1, PLAYER_HPTEXT_X, PLAYER_HPTEXT_Y, OPAQUE_TEXT);

        disp_p->redraw = false;
    }

    // Move the player
    GFX_setForeground(&disp_p->gfx, game_state_p->player.color);
    MoveRectangle(hal_p, disp_p, &game_state_p->player.body, &game_state_p->player.motion);

    // Move the bullets
    uint8_t i;
    for (i = 0; i < MAX_PLAYER_BULLETS; i++) {
        MoveRectangle(hal_p, disp_p, &game_state_p->player_bullet[i].body, &game_state_p->player_bullet[i].motion);
    }

    // Logic for shooting bullets
    if (Button_isTapped(&hal_p->boosterpackS1)) {
        for (i = 0; i < MAX_PLAYER_BULLETS; i++) {
            if (!game_state_p->player_bullet[i].motion.exists) {
                game_state_p->player_bullet[i].motion.exists = true;

                game_state_p->player_bullet[i].body.xMin = AVG(game_state_p->player.body.xMin, game_state_p->player.body.xMax);
                game_state_p->player_bullet[i].body.xMax = game_state_p->player_bullet[i].body.xMin + PROJECTILE_WIDTH;
                game_state_p->player_bullet[i].body.yMax = game_state_p->player.body.yMin;
                game_state_p->player_bullet[i].body.yMin = game_state_p->player_bullet[i].body.yMax - PROJECTILE_LENGTH;

                SWTimer_start(&game_state_p->player_bullet[i].motion.mTimer);
                break;
            }
        }
    }
}
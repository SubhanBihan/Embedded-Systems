#include "Display.h"
// #include <Display.h>

uint32_t CircularIncrement(uint32_t value, uint32_t maximum) {
  return (value + 1) % maximum;
}

uint32_t CircularDecrement(uint32_t value, uint32_t maximum) {
    return value = (value + maximum - 1) % maximum;
}

unsigned GetRectangleLength(const Graphics_Rectangle* R) { return R->yMax - R->yMin; }
unsigned GetRectangleWidth(const Graphics_Rectangle* R) { return R->xMax - R->xMin; }

bool IsPointInRect(const int16_t x_p, const int16_t y_p, const Graphics_Rectangle* R) {
    return ((x_p >= R->xMin) && (x_p <= R->xMax) && (y_p >= R->yMin) && (y_p <= R->yMax));
}

bool IsOverlap(const Graphics_Rectangle* Line, const Graphics_Rectangle* Rect) {
    return (IsPointInRect(Line->xMin, Line->yMin, Rect) || IsPointInRect(Line->xMax, Line->yMax, Rect));
}

bool IsOutOfBounds(const Graphics_Rectangle* R1, const Graphics_Rectangle* Grid) {
    return ((R1->xMax >= Grid->xMax) || (R1->xMin <= Grid->xMin) || (R1->yMax >= Grid->yMax) || (R1->yMin <= Grid->yMin));
}

bool IsOutOfBounds_X(const Graphics_Rectangle* R1, const Graphics_Rectangle* Grid) {
    return ((R1->xMax >= Grid->xMax) || (R1->xMin <= Grid->xMin));
}

bool IsOutOfBounds_Y(const Graphics_Rectangle* R1, const Graphics_Rectangle* Grid) {
    return ((R1->yMax >= Grid->yMax) || (R1->yMin <= Grid->yMin));
}

void KeepInBounds(Graphics_Rectangle* R, const Graphics_Rectangle* Grid) {
    if (R->xMin < Grid->xMin) {
        R->xMin = Grid->xMin; R->xMax = R->xMin + CHARACTER_LENGTH;
    }
    else if (R->xMax > Grid->xMax) {
        R->xMax = Grid->xMax;  R->xMin = R->xMax - CHARACTER_LENGTH;
    }

    if (R->yMin < Grid->yMin) {
        R->yMin = Grid->yMin; R->yMax = R->yMin + CHARACTER_LENGTH;
    }
    else if (R->yMax > Grid->yMax) {
        R->yMax = Grid->yMax;  R->yMin = R->yMax - CHARACTER_LENGTH;
    }
}

unsigned GetScore(int player_HP, unsigned game_time) {
    return sqrt(game_time) * player_HP * player_HP * (player_HP > 0);     // Rewards HP retention much more than game time
}

void MoveRectangle(Graphics_Rectangle* R, const motion_t* motion_p) {
    R->xMin += motion_p->left_right * motion_p->dx;
    R->xMax += motion_p->left_right * motion_p->dx;
    R->yMin += motion_p->up_down    * motion_p->dy;
    R->yMax += motion_p->up_down    * motion_p->dy;
}

void MovePlayer(HAL* hal_p, display_t* disp_p, Graphics_Rectangle* R, motion_t* motion_p) {
    // Defines the Grid area
    static const Graphics_Rectangle Grid = {GRID_X_START, GRID_Y_START, GRID_X_END, GRID_Y_END};

    if (SWTimer_expired(&motion_p->mTimer)) {
        unsigned vx, vy;

        // Erase the old position image
        GFX_removeSolidSquare(&disp_p->gfx, R);

        // Determine movement directions
        getSampleJoyStick(&vx, &vy);
        motion_p->left_right = vx < LEFT_THRESHOLD ? MINUS : vx > RIGHT_THRESHOLD ? PLUS  : ZERO; 
        motion_p->up_down    = vy < DOWN_THRESHOLD ? PLUS  : vy > UP_THRESHOLD    ? MINUS : ZERO; 
        
        // Control the Horiontal and Vertical motion
        MoveRectangle(R, motion_p);

        // If player has gone outside bounds, adjust postion
        KeepInBounds(R, &Grid);

        // Draw the new position image
        GFX_drawSolidSquare(&disp_p->gfx, R); 

        // Restart the timer
        SWTimer_start(&motion_p->mTimer);
    }


}

void MoveEnemy(display_t* disp_p, Graphics_Rectangle* R, motion_t* motion_p) {
    // Defines the Grid area
    static const Graphics_Rectangle Grid = {GRID_X_START, GRID_Y_START, GRID_X_END, GRID_Y_END};

    static int d_X = -1, d_Y = -1;        // Distances for sporadic movement

    if (SWTimer_expired(&motion_p->mTimer)) {
        // Erase the old position image
        GFX_removeSolidSquare(&disp_p->gfx, R);

        // Conditions for direction change and "re-roll"
        if (IsOutOfBounds_X(R, &Grid) || d_X <= 0) {
            // Renew distances to move
            d_X = 1 + rand() % MAX_X_DIST;

            // Reverse movement directions
            motion_p->left_right *= MINUS;
        }

        if (IsOutOfBounds_Y(R, &Grid) || d_Y <= 0) {
            // Renew distances to move
            d_Y = 1 + rand() % MAX_Y_DIST;

            // Reverse movement directions
            motion_p->up_down    *= MINUS;
        }

        // Control the Horiontal and Vertical motion
        MoveRectangle(R, motion_p);
        d_X     += motion_p->left_right * motion_p->dx;
        d_Y     += motion_p->up_down    * motion_p->dy;

        // If enemy has gone outside bounds, adjust postion
        KeepInBounds(R, &Grid);

        // Draw the new position image
        GFX_drawSolidSquare(&disp_p->gfx, R); 

        // Restart the timer
        SWTimer_start(&motion_p->mTimer);   
    }

}

void MoveBullet(display_t* disp_p, Graphics_Rectangle* R, motion_t* motion_p) {
    // Defines the Grid area
    static const Graphics_Rectangle Grid = {GRID_X_START, GRID_Y_START, GRID_X_END, GRID_Y_END};

    if (SWTimer_expired(&motion_p->mTimer)) {
        // Erase the old position image
        // GFX_removeSolidSquare(&disp_p->gfx, R);
        GFX_removeLine(&disp_p->gfx, R->xMin, R->xMax, R->yMin, R->yMax);

        if (motion_p->exists) {
            // Control the Horiontal and Vertical motion
            MoveRectangle(R, motion_p);


            if (IsOverlap(R, &Grid))           
                // Draw the new position image
                // GFX_drawSolidSquare(&disp_p->gfx, R); 
                GFX_drawLine(&disp_p->gfx, R->xMin, R->xMax, R->yMin, R->yMax);
            else
                // If the object is beyond the grid region, it no longer exists
                motion_p->exists = false;

            SWTimer_start(&motion_p->mTimer);   // Restart the timer
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

void MainScreen_Text(display_t* disp_p, uint8_t* j) {
    static const char* lines[] = {"  New Game", "  Resume Game", "  Instructions", "  High Scores", "BB1: Cursor Up", "BB2: Cursor Down", "JSB: Select"};
    static const uint8_t num_lines = sizeof(lines) / sizeof(char*);

    Graphics_clearDisplay(&disp_p->gfx.context);
    GFX_setForeground(&disp_p->gfx, TEXT_COLOR);   // Text color
    
    uint8_t i = 0;

    Graphics_drawString(&disp_p->gfx.context, (int8_t *)"MENU", -1, 5, MIN_LINE_SPACING * ++i, true);

    *j = i;

    for ( ; i - (*j) < num_lines; i++) {
        Graphics_drawString(&disp_p->gfx.context, lines[i - (*j)], -1, 5, MIN_LINE_SPACING * (i + 1), true);
    }
}

void MainScreen(HAL* hal_p, display_t* disp_p) {
    static option_t option_num;
    static uint8_t j;

    // Upon entry into function
    if (disp_p->screen_changed) {
        MainScreen_Text(&disp_p->gfx.context, &j);

        disp_p->screen_changed = false;
        disp_p->redraw = true;
        option_num = OPTION_ONE;
    }

    // Move cursor up
    if (Button_isTapped(&hal_p->boosterpackS1)) {
        // prev_option = option_num;
        option_num = CircularDecrement(option_num, NUM_OPTIONS);
        disp_p->redraw = true;
    }

    // Move cursor down
    if (Button_isTapped(&hal_p->boosterpackS2)) {
        // prev_option = option_num;
        option_num = CircularIncrement(option_num, NUM_OPTIONS);
        disp_p->redraw = true;
    }


    // Change the option arrow
    if (disp_p->redraw) {
        GFX_setForeground(&disp_p->gfx, ARROW_COLOR);

        uint8_t k;

        for (k = 0; k < NUM_OPTIONS; k++) {
            if (k == option_num)
                Graphics_drawString(&disp_p->gfx.context, (int8_t *)">", -1, 5, MIN_LINE_SPACING * (j + k + 1), true);
            else
                Graphics_drawString(&disp_p->gfx.context, (int8_t *)"  ", -1, 5, MIN_LINE_SPACING * (j + k + 1), true);
        }

        disp_p->redraw = false;
    }


    // Select the indicated option
    if (Button_isTapped(&hal_p->boosterpackJS)) {
        switch (option_num)
        {
            case OPTION_ONE:
                disp_p->screen = GAME;
                break;
            case OPTION_TWO:
                disp_p->screen = RESUME;
                break;
            case OPTION_THREE:
                disp_p->screen = INSTRUCTIONS;
                break;
            case OPTION_FOUR:
                disp_p->screen = HS;
                break;
        }
        disp_p->screen_changed = true;
    }

}


void InstructionsScreen(HAL* hal_p, display_t* disp_p) {
    // Instructions text
    static const char* lines[] = {"Move the character", "to avoid being hit by", "the bullets.", "Press BB1 to fire at", "the enemy.", "First one to run out", "of health loses."};
    static const uint8_t num_lines = sizeof(lines) / sizeof(char*);

    if (disp_p->screen_changed) {   // Only do these when first entering main screen (function)
        // MainScreen_Text(&disp_p->gfx.context, &j);

        // Clear the display
        GFX_clear(&disp_p->gfx);

        // Set foreground color
        GFX_setForeground(&disp_p->gfx, TEXT_COLOR);

        uint8_t i = 1;  // Tracking row of current line

        // Print Screen name
        GFX_print(&disp_p->gfx, "INSTRUCTIONS", i, 1);
        i += MIN_SECTION_GAP_ROWS;

        // Print instructions
        i = GFX_printTextRows(&disp_p->gfx, lines, num_lines, i, 1);
        i += MIN_SECTION_GAP_ROWS;

        // Print navigations instructions
        GFX_print(&disp_p->gfx, "Press BB2 to return", i, 1);

        // Set flags to stay on this screen
        disp_p->screen_changed = false;
    }

    if (Button_isTapped(&hal_p->boosterpackS2)) {
        disp_p->screen = MAIN;
        disp_p->screen_changed = true;
    }
}

void HSScreen(HAL* hal_p, display_t* disp_p, game_state_t* game_state_p) {
    if (disp_p->screen_changed) {   // Only do these when first entering main screen (function)
        // Clear the display
        GFX_clear(&disp_p->gfx);

        // Set foreground color
        GFX_setForeground(&disp_p->gfx, TEXT_COLOR);

        uint8_t i = 1;  // Tracking row of current line

        // Print Screen name
        GFX_print(&disp_p->gfx, "HIGH SCORES", i, 1);
        // i += MIN_SECTION_GAP_ROWS;

        // Display the scores
        char score_string[50];
        for (i = 0; i < NUM_SCORES; i++) {
            snprintf(score_string, sizeof(score_string), "%d. %d", i + 1, game_state_p->high_scores[i]);
            GFX_print(&disp_p->gfx, score_string, i + 2, 2);
        }

        GFX_print(&disp_p->gfx, "BB2: Go Back", i + 5, 1);

        // Set flags to stay on this screen
        disp_p->screen_changed = false;
    }

    // Exit
    if (Button_isTapped(&hal_p->boosterpackS2)) {
        disp_p->screen = MAIN;
        disp_p->screen_changed = true;
    }
}

unsigned GetNumEnemyBullets(attack_t enemy_pattern) {
    switch (enemy_pattern)
        {
            case VERTICAL:
                return (GRID_X_RANGE / ENEMY_BULLET_SPACING);
    

            case HORIZONTAL:
                return (GRID_Y_RANGE / ENEMY_BULLET_SPACING);
                
            case DIAGONAL:
                return (GRID_Y_RANGE / ENEMY_BULLET_SPACING);
                
            
            case DUAL_HORIZONTAL:
                return (2 * GRID_Y_RANGE / ENEMY_BULLET_SPACING);
                
            
            default:
                break;
        }
}

void NewGameInit(display_t* disp_p, game_state_t* game_state_p, unsigned* num_enemy_bullets, SWTimer* game_timer, SWTimer* pattern_timer, SWTimer* attack_timer, bool* player_power, bool* enemy_power, Graphics_Rectangle* player_HPB, Graphics_Rectangle* enemy_HPB, SWTimer* player_power_timer) {
    game_state_p->new_game = false;
    game_state_p->time = 0;

    // Random seed
    srand(Timer32_getValue(TIMER32_0_BASE));

    // ***** Set the Initial Game State *****
    // For player
    game_state_p->player.body.xMin = PLAYER_INIT_XMIN;
    game_state_p->player.body.xMax = game_state_p->player.body.xMin + CHARACTER_LENGTH;
    game_state_p->player.body.yMin = PLAYER_INIT_YMIN;
    game_state_p->player.body.yMax = game_state_p->player.body.yMin + CHARACTER_LENGTH;

    game_state_p->player.color = PLAYER_COLOR;
    game_state_p->player.HP = PLAYER_INIT_HP;
    game_state_p->player.motion = (motion_t) {true, ZERO, ZERO, CHARACTER_DX, CHARACTER_DY, SWTimer_construct(CHARACTER_REFRESH_MS), CHARACTER_REFRESH_MS};

    // For Enemy
    game_state_p->enemy.body.xMin = ENEMY_INIT_XMIN;
    game_state_p->enemy.body.xMax = game_state_p->enemy.body.xMin + CHARACTER_LENGTH;
    game_state_p->enemy.body.yMin = ENEMY_INIT_YMIN;
    game_state_p->enemy.body.yMax = game_state_p->enemy.body.yMin + CHARACTER_LENGTH;

    game_state_p->enemy.color = ENEMY_COLOR;
    game_state_p->enemy.HP = ENEMY_INIT_HP;
    game_state_p->enemy.motion = (motion_t) {true, ZERO, PLUS, CHARACTER_DX, CHARACTER_DY, SWTimer_construct(CHARACTER_REFRESH_MS), CHARACTER_REFRESH_MS};

    uint16_t i = 0;
    
    // For player bullets
    for (i = 0; i < MAX_PLAYER_BULLETS; i++) {
        game_state_p->player_bullet[i].motion = (motion_t) {false, MINUS, ZERO, PROJECTILE_DX, PROJECTILE_DY, SWTimer_construct(PROJECTILE_REFRESH_MS), PROJECTILE_REFRESH_MS};
    }

    // For enemy bullets
    for (i = 0; i < MAX_ENEMY_BULLETS; i++) {
        game_state_p->enemy_bullet[i].motion = (motion_t) {false, PLUS, ZERO, PROJECTILE_DX, PROJECTILE_DY, SWTimer_construct(PROJECTILE_REFRESH_MS), PROJECTILE_REFRESH_MS};
    }

    // Initial enemy attack pattern
    EnemyChangePattern(disp_p, game_state_p);

    // Messy pointers ahead!!!
    *num_enemy_bullets = GetNumEnemyBullets(game_state_p->enemy_pattern);

    // Turn off power-ups
    *enemy_power = false;
    *player_power = false;

    // Set initial HP Bars
    *player_HPB = (Graphics_Rectangle) {PLAYER_HPBAR_XMIN, PLAYER_HPBAR_YMIN, PLAYER_HPBAR_XMAX, PLAYER_HPBAR_YMAX};
    *enemy_HPB = (Graphics_Rectangle) {ENEMY_HPBAR_XMIN, ENEMY_HPBAR_YMIN, ENEMY_HPBAR_XMAX, ENEMY_HPBAR_YMAX};

    // Construct player power timer
    *player_power_timer = SWTimer_construct(PLAYER_POWER_DURATION_MS);

    // Construct and start game timer
    *game_timer = SWTimer_construct(GAME_TIMER_MS);
    SWTimer_start(game_timer);

    // Construct and start timers for enemy's attacks
    *pattern_timer = SWTimer_construct(ENEMY_MID_PATTERN_MS + MAX_BULLET_TIME_MS);
    *attack_timer = SWTimer_construct(ENEMY_MID_ATTACK_MS);

    SWTimer_start(pattern_timer);
    SWTimer_start(attack_timer);

    // Start the player's and enemy's motion timer
    SWTimer_start(&game_state_p->player.motion.mTimer);
    SWTimer_start(&game_state_p->enemy.motion.mTimer);
}

void MovePlayerBullets(display_t* disp_p, game_state_t* game_state_p, bool enemy_power) {
    uint8_t i;
    for (i = 0; i < MAX_PLAYER_BULLETS; i++) {
        if (game_state_p->player_bullet[i].motion.exists) {
            // If the bullet collides with enemy, make bullet disappear and reduce enemy HP
            if (IsOverlap(&game_state_p->player_bullet[i].body, &game_state_p->enemy.body)) {
                game_state_p->player_bullet[i].motion.exists = false;
                // GFX_removeSolidSquare(&disp_p->gfx, &game_state_p->player_bullet[i].body);
                GFX_removeLine(&disp_p->gfx, &game_state_p->player_bullet[i].body.xMin, &game_state_p->player_bullet[i].body.xMax, &game_state_p->player_bullet[i].body.yMin, &game_state_p->player_bullet[i].body.yMax);
                game_state_p->enemy.HP -= enemy_power ? ENEMY_POWER_DAMAGE : PROJECTILE_DAMAGE;

                disp_p->redraw = true;
            }

            // Move the bullet
            MoveBullet(disp_p, &game_state_p->player_bullet[i].body, &game_state_p->player_bullet[i].motion);
        }
    }
}

void ShootPlayerBullet(HAL* hal_p, game_state_t* game_state_p) {
    uint8_t i;
    if (Button_isTapped(&hal_p->boosterpackS1)) {
        for (i = 0; i < MAX_PLAYER_BULLETS; i++) {
            if (!game_state_p->player_bullet[i].motion.exists) {    // Create a bullet at the lowest possible index
                game_state_p->player_bullet[i].motion.exists = true;

                game_state_p->player_bullet[i].body.xMin = AVG(game_state_p->player.body.xMin, game_state_p->player.body.xMax);
                game_state_p->player_bullet[i].body.xMax = game_state_p->player_bullet[i].body.xMin;
                game_state_p->player_bullet[i].body.yMax = game_state_p->player.body.yMin;
                game_state_p->player_bullet[i].body.yMin = game_state_p->player_bullet[i].body.yMax - PROJECTILE_LENGTH;

                SWTimer_start(&game_state_p->player_bullet[i].motion.mTimer);       // Start bullet timer
                break;
            }
        }
    }
}

void MoveEnemyBullets(display_t* disp_p, game_state_t* game_state_p, const unsigned num_enemy_bullets, bool player_power) {
    uint8_t i;
    for (i = 0; i < num_enemy_bullets; i++) {
        if (game_state_p->enemy_bullet[i].motion.exists) {
            // If the bullet collides with player, make bullet disappear and reduce player HP
            if (IsOverlap(&game_state_p->enemy_bullet[i].body, &game_state_p->player.body)) {
                game_state_p->enemy_bullet[i].motion.exists = false;
                // GFX_removeSolidSquare(&disp_p->gfx, &game_state_p->enemy_bullet[i].body);
                GFX_removeLine(&disp_p->gfx, &game_state_p->enemy_bullet[i].body.xMin, &game_state_p->enemy_bullet[i].body.xMax, &game_state_p->enemy_bullet[i].body.yMin, &game_state_p->enemy_bullet[i].body.yMax);
                game_state_p->player.HP -= player_power ? 0 : PROJECTILE_DAMAGE;

                disp_p->redraw = true;
            }

            MoveBullet(disp_p, &game_state_p->enemy_bullet[i].body, &game_state_p->enemy_bullet[i].motion);     // Move the bullet
        }
    }
}

void EnemySinglePattern(game_state_t* game_state_p, unsigned* j) {
    // Bullet now exists
    game_state_p->enemy_bullet[(*j)].motion.exists = true;

    // Set its initial position
    switch (game_state_p->enemy_pattern)
    {
        case VERTICAL:
            game_state_p->enemy_bullet[(*j)].body.xMin = GRID_X_START + (*j) * ENEMY_BULLET_SPACING;
            game_state_p->enemy_bullet[(*j)].body.xMax = game_state_p->enemy_bullet[(*j)].body.xMin;
            game_state_p->enemy_bullet[(*j)].body.yMin = GRID_Y_START;
            game_state_p->enemy_bullet[(*j)].body.yMax = game_state_p->enemy_bullet[(*j)].body.yMin + PROJECTILE_LENGTH;
            break;

        case HORIZONTAL:
            game_state_p->enemy_bullet[(*j)].body.xMin = GRID_X_START;
            game_state_p->enemy_bullet[(*j)].body.xMax = game_state_p->enemy_bullet[(*j)].body.xMin + PROJECTILE_LENGTH;
            game_state_p->enemy_bullet[(*j)].body.yMin = GRID_Y_START + (*j) * ENEMY_BULLET_SPACING;
            game_state_p->enemy_bullet[(*j)].body.yMax = game_state_p->enemy_bullet[(*j)].body.yMin;
            break;
        
        case DIAGONAL:
            game_state_p->enemy_bullet[(*j)].body.xMin = GRID_X_START;
            game_state_p->enemy_bullet[(*j)].body.xMax = ceil(game_state_p->enemy_bullet[(*j)].body.xMin + PROJECTILE_LENGTH * SQRT2);
            game_state_p->enemy_bullet[(*j)].body.yMin = GRID_Y_START + (*j) * ENEMY_BULLET_SPACING;
            game_state_p->enemy_bullet[(*j)].body.yMax = ceil(game_state_p->enemy_bullet[(*j)].body.yMin + PROJECTILE_LENGTH * SQRT2);
            break;
        
        case DUAL_HORIZONTAL:
            // Bullet from left (should move RIGHT)
            game_state_p->enemy_bullet[(*j)].motion.left_right = PLUS;
            game_state_p->enemy_bullet[(*j)].motion.up_down = ZERO;
            game_state_p->enemy_bullet[(*j)].body.xMin = GRID_X_START;
            game_state_p->enemy_bullet[(*j)].body.xMax = game_state_p->enemy_bullet[(*j)].body.xMin + PROJECTILE_LENGTH;
            game_state_p->enemy_bullet[(*j)].body.yMin = GRID_Y_START + 2 * ((*j) / 2) * ENEMY_BULLET_SPACING;
            game_state_p->enemy_bullet[(*j)].body.yMax = game_state_p->enemy_bullet[(*j)].body.yMin;
            game_state_p->enemy_bullet[(*j)].motion.exists = true;
            
            SWTimer_start(&game_state_p->enemy_bullet[(*j)].motion.mTimer);
            (*j)++;

            // Bullet from right (should move LEFT)
            game_state_p->enemy_bullet[(*j)].motion.left_right = MINUS;
            game_state_p->enemy_bullet[(*j)].motion.up_down = ZERO;
            game_state_p->enemy_bullet[(*j)].body.xMax = GRID_X_END;
            game_state_p->enemy_bullet[(*j)].body.xMin = game_state_p->enemy_bullet[(*j)].body.xMax - PROJECTILE_LENGTH;
            game_state_p->enemy_bullet[(*j)].body.yMin = GRID_Y_START + 2 * ((*j) / 2) * ENEMY_BULLET_SPACING;
            game_state_p->enemy_bullet[(*j)].body.yMax = game_state_p->enemy_bullet[(*j)].body.yMin;
            game_state_p->enemy_bullet[(*j)].motion.exists = true;
            break;
                    break;
        default:
            break;
    }
        
    SWTimer_start(&game_state_p->enemy_bullet[(*j)].motion.mTimer);        // Start the bullet's motion timer
    (*j)++;                                                              // Move to next bullet
}

void EnemyChangePattern(display_t* disp_p, game_state_t* game_state_p) {
    uint16_t i;

    game_state_p->enemy_pattern = rand() % NUM_PATTERNS;     // Move to a random pattern

    // Reset bullet existence and directions
    switch (game_state_p->enemy_pattern)
    {
        case VERTICAL:
            for (i = 0; i < MAX_ENEMY_BULLETS; i++) {
                GFX_removeLine(&disp_p->gfx, game_state_p->enemy_bullet[i].body.xMin, game_state_p->enemy_bullet[i].body.xMax, game_state_p->enemy_bullet[i].body.yMin, game_state_p->enemy_bullet[i].body.yMax);
                game_state_p->enemy_bullet[i].motion.exists = false;

                game_state_p->enemy_bullet[i].motion.left_right = ZERO;
                game_state_p->enemy_bullet[i].motion.up_down = PLUS;
            }
            break;

        case HORIZONTAL:
            for (i = 0; i < MAX_ENEMY_BULLETS; i++) {
                GFX_removeLine(&disp_p->gfx, game_state_p->enemy_bullet[i].body.xMin, game_state_p->enemy_bullet[i].body.xMax, game_state_p->enemy_bullet[i].body.yMin, game_state_p->enemy_bullet[i].body.yMax);
                game_state_p->enemy_bullet[i].motion.exists = false;

                game_state_p->enemy_bullet[i].motion.left_right = PLUS;
                game_state_p->enemy_bullet[i].motion.up_down = ZERO;
            }
            break;

        case DIAGONAL:
            for (i = 0; i < MAX_ENEMY_BULLETS; i++) {
                GFX_removeLine(&disp_p->gfx, game_state_p->enemy_bullet[i].body.xMin, game_state_p->enemy_bullet[i].body.xMax, game_state_p->enemy_bullet[i].body.yMin, game_state_p->enemy_bullet[i].body.yMax);
                game_state_p->enemy_bullet[i].motion.exists = false;

                game_state_p->enemy_bullet[i].motion.left_right = PLUS;
                game_state_p->enemy_bullet[i].motion.up_down = PLUS;
            }
            break;
        
        case DUAL_HORIZONTAL:
            for (i = 0; i < MAX_ENEMY_BULLETS; i++) {
                GFX_removeLine(&disp_p->gfx, game_state_p->enemy_bullet[i].body.xMin, game_state_p->enemy_bullet[i].body.xMax, game_state_p->enemy_bullet[i].body.yMin, game_state_p->enemy_bullet[i].body.yMax);
                game_state_p->enemy_bullet[i].motion.exists = false;

                // game_state_p->enemy_bullet[i].motion.left_right = (i % 2) ? MINUS : PLUS;
                // game_state_p->enemy_bullet[i].motion.up_down = ZERO;
            }
            break;
        
        default:
            break;
    }
    
}

void GameScreenGraphics(display_t* disp_p, game_state_t* game_state_p, Graphics_Rectangle* player_HPB, Graphics_Rectangle* enemy_HPB) {
    char line[20];

    // Remove Old HP Bars
    GFX_removeSolidSquare(&disp_p->gfx, player_HPB);
    GFX_removeSolidSquare(&disp_p->gfx, enemy_HPB);

    // Adjust the HPBs
    float hp_fraction = (float)max(0, game_state_p->player.HP) / PLAYER_INIT_HP;
    int hp_pixels = (int) roundf(PLAYER_HPBAR_YRANGE * hp_fraction);
    player_HPB->yMin = player_HPB->yMax - hp_pixels;

    hp_fraction = (float)max(0, game_state_p->enemy.HP) / ENEMY_INIT_HP;
    hp_pixels = (int) roundf(ENEMY_HPBAR_YRANGE * hp_fraction);
    enemy_HPB->yMin = enemy_HPB->yMax - hp_pixels;

    // Draw Player HP circle (HPC) and bar (HPB)
    GFX_setForeground(&disp_p->gfx, game_state_p->player.color);
    GFX_drawSolidCircle(&disp_p->gfx, PLAYER_HPC_X, PLAYER_HPC_Y, HPC_RADIUS);
    GFX_drawSolidSquare(&disp_p->gfx, player_HPB);

    // Draw Enemy HP circle (HPC) and bar (HPB)
    GFX_setForeground(&disp_p->gfx, game_state_p->enemy.color);
    GFX_drawSolidCircle(&disp_p->gfx, ENEMY_HPC_X, ENEMY_HPC_Y, HPC_RADIUS);
    GFX_drawSolidSquare(&disp_p->gfx, enemy_HPB);

    // Set foreground color
    GFX_setForeground(&disp_p->gfx, TEXT_COLOR);

    // Player HP Text
    snprintf(line, sizeof(line), "%3d", max(0, game_state_p->player.HP));
    // GFX_print(&disp_p->gfx, line, PLAYER_HPTEXT_X, PLAYER_HPTEXT_Y);
    Graphics_drawString(&disp_p->gfx.context, line, -1, PLAYER_HPTEXT_X, PLAYER_HPTEXT_Y, OPAQUE_TEXT);

    // Enemy HP Text
    snprintf(line, sizeof(line), "%3d", max(0, game_state_p->enemy.HP));
    // GFX_print(&disp_p->gfx, line, ENEMY_HPTEXT_X, ENEMY_HPTEXT_Y);
    Graphics_drawString(&disp_p->gfx.context, line, -1, ENEMY_HPTEXT_X, ENEMY_HPTEXT_Y, OPAQUE_TEXT);

    disp_p->redraw = false;
}

void GameScreen(HAL* hal_p, display_t* disp_p, game_state_t* game_state_p) {
    char line[20];

    static unsigned num_enemy_bullets;

    static SWTimer game_timer, pattern_timer, attack_timer, player_power_timer;

    static bool player_power, enemy_power;

    static Graphics_Rectangle player_HPB, enemy_HPB;

    static uint8_t j = 0;   // For tracking enemy bullet number

    if (disp_p->screen_changed) {   // Only do these when first entering main screen (function)
        // Clear the display
        GFX_clear(&disp_p->gfx);

        if (game_state_p->new_game) {
            NewGameInit(disp_p, game_state_p, &num_enemy_bullets, &game_timer, &pattern_timer, &attack_timer, &player_power, &enemy_power, &player_HPB, &enemy_HPB, &player_power_timer);
            j = 0;
        }

        // Set flags to stay on this screen
        disp_p->screen_changed = false;
        disp_p->redraw = true;
    }


    // Move the player
    GFX_setForeground(&disp_p->gfx, game_state_p->player.color);
    MovePlayer(hal_p, disp_p, &game_state_p->player.body, &game_state_p->player.motion);

    // Move the player bullets
    MovePlayerBullets(disp_p, game_state_p, enemy_power);

    // Logic for shooting (player) bullets
    ShootPlayerBullet(hal_p, game_state_p);

    // Check if enemy power-up condition met
    enemy_power = (game_state_p->enemy.HP <= ENEMY_POWER_HP);
    if (enemy_power) game_state_p->enemy.color = ENEMY_POWER_COLOR;

    // Move the enemy
    GFX_setForeground(&disp_p->gfx, game_state_p->enemy.color);
    MoveEnemy(disp_p, &game_state_p->enemy.body, &game_state_p->enemy.motion);

    // Move the enemy bullets
    MoveEnemyBullets(disp_p, game_state_p, num_enemy_bullets, player_power);

    // Logic for creating enemy bullets
    // Generate bullets (only if pattern timer hasn't expired)
    // Once pattern delay expires, change pattern
    if (j >= num_enemy_bullets && SWTimer_expired(&pattern_timer)) {
        j = 0;
        EnemyChangePattern(disp_p, game_state_p);
        num_enemy_bullets = GetNumEnemyBullets(game_state_p->enemy_pattern);
        // Don't restart pattern_timer here - it will be restarted when last bullet spawns
    }

    // Spawn bullets
    if (j < num_enemy_bullets && SWTimer_expired(&attack_timer)) {
        EnemySinglePattern(game_state_p, &j);
        SWTimer_start(&attack_timer);
        
        // When we spawn the LAST bullet, start the pattern delay timer
        if (j >= num_enemy_bullets) {
            SWTimer_start(&pattern_timer);
        }
    }

    // Print Game Time
    if (SWTimer_expired(&game_timer)) {
        // Set foreground color
        GFX_setForeground(&disp_p->gfx, TEXT_COLOR);

        snprintf(line, sizeof(line), "Time: %d", game_state_p->time);
        GFX_print(&disp_p->gfx, line, 1, 0);

        game_state_p->time++;

        SWTimer_start(&game_timer);
    }

    // Check if player power-up criterion 
    if (!((game_state_p->time + 1) % PLAYER_POWER_INTERVAL) && !player_power) {
        player_power = true;
        game_state_p->player.color = PLAYER_POWER_COLOR;
        SWTimer_start(&player_power_timer);
    }

    // Check if player power-up time is up
    if (player_power && SWTimer_expired(&player_power_timer)) {
        player_power = false;
        game_state_p->player.color = PLAYER_COLOR;
    }

    // Redraw Graphics
    if (disp_p->redraw) {
        GameScreenGraphics(disp_p, game_state_p, &player_HPB, &enemy_HPB);
    }

    // Condition for Game Over
    if (game_state_p->player.HP <= 0 || game_state_p->enemy.HP <= 0) {
        disp_p->screen = GAMEOVER;
        disp_p->screen_changed = true;
        game_state_p->new_game = true;
    }

    // Exit the game
    if (Button_isTapped(&hal_p->boosterpackS2)) {
        disp_p->screen = MAIN;
        disp_p->screen_changed = true;
    }
    
}


// Game Over Screen
void GameOverScreen(HAL* hal_p, display_t* disp_p, game_state_t* game_state_p) {
    char line[50];
    if (disp_p->screen_changed) {   // Only do these when first entering main screen (function)
        // Set foreground color
        GFX_setForeground(&disp_p->gfx, TEXT_COLOR);

        uint8_t i = 0, j = 0;

        // Print Winner message
        if (game_state_p->player.HP <= 0)
            GFX_print(&disp_p->gfx, " You LOSE :X", 6, 4);
        else
            GFX_print(&disp_p->gfx, " You WIN =)", 6, 4);
        // i += MIN_SECTION_GAP_ROWS;

        game_state_p->score = GetScore(game_state_p->player.HP, game_state_p->time);
        
        snprintf(line, sizeof(line), "Score: %d", game_state_p->score);
        GFX_print(&disp_p->gfx, line, 12, 0);
        GFX_print(&disp_p->gfx, "Press JSB to continue", 13, 0);

        // Update the high scores
        if (game_state_p->score > game_state_p->high_scores[NUM_SCORES - 1])         // No need to update if this isn't met
            for (i = 0; i < NUM_SCORES; i++) {
                if (game_state_p->score >= game_state_p->high_scores[i]) {
                    for (j = NUM_SCORES - 1; j > i; j--) {
                        game_state_p->high_scores[j] = game_state_p->high_scores[j-1];
                    }
                    game_state_p->high_scores[i] = game_state_p->score;

                    break;
                }
            }

        // Set flags to stay on this screen
        disp_p->screen_changed = false;
    }

    if (Button_isTapped(&hal_p->boosterpackJS)) {
        disp_p->screen = MAIN;
        disp_p->screen_changed = true;
    }
}
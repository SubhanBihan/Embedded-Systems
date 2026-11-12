#include <Application.h>

void ScreenSelect(HAL* hal_p, Application* app_p) {
    switch (app_p->disp.screen)
    {
        case TITLE:
            TitleScreen(hal_p, &app_p->disp);
            break;
        
        case MAIN:
            MainScreen(hal_p, &app_p->disp);
            break;

        case INSTRUCTIONS:
            InstructionsScreen(hal_p, &app_p->disp);
            break;
        
        case HS:
            HSScreen(hal_p, &app_p->disp);
            break;
        
        case GAME:
            GameScreen(hal_p, &app_p->disp, &app_p->game_state);
            break;
        
        // case GAMEOVER:
        //     GameOverScreen(hal_p, &app_p->disp);
        //     break;
        
        default:
            break;
    }
}

void Application_construct(Application* app_p) {
    uint8_t i;

    // Set the display struct
    app_p->disp.gfx = GFX_construct(TEXT_COLOR, BACKGROUND_COLOR);
    // GFX_resetColors(&app_p->disp.gfx);
    app_p->disp.screen_changed = true;
    app_p->disp.redraw = true;
    app_p->disp.screen = TITLE;

    // ***** Set the Initial Game State *****
    // For player
    app_p->game_state.player.body.xMin = PLAYER_INIT_XMIN;
    app_p->game_state.player.body.xMax = app_p->game_state.player.body.xMin + CHARACTER_LENGTH;
    app_p->game_state.player.body.yMin = PLAYER_INIT_YMIN;
    app_p->game_state.player.body.yMax = app_p->game_state.player.body.yMin + CHARACTER_LENGTH;

    app_p->game_state.player.color = PLAYER_COLOR;
    app_p->game_state.player.HP = PLAYER_INIT_HP;
    app_p->game_state.player.motion = (motion_t) {true, true, false, true, 1, 1, CHARACTER_DX, CHARACTER_DY, SWTimer_construct(CHARACTER_REFRESH_MS), CHARACTER_REFRESH_MS};

    // For bullets
    for (i = 0; i < MAX_PLAYER_BULLETS; i++) {
        app_p->game_state.player_bullet[i].motion = (motion_t) {false, false, false, true, -1, 0, PROJECTILE_DX, PROJECTILE_DY, SWTimer_construct(PROJECTILE_REFRESH_MS), PROJECTILE_REFRESH_MS};
    }

    // Set high scores to 0
    for (i = 0; i < NUM_SCORES; i++) app_p->game_state.high_scores[i] = 0;

}


void Application_loop(HAL* hal_p, Application* app_p) {
    // // Restart/Update communications if first run or S2 is pressed
    // if (Button_isTapped(&hal_p->launchpadS2) || app_p->firstCall) {
    //     Application_updateCommunications(app_p, hal_p);
    // }

    ScreenSelect(hal_p, app_p);

    // if (app_p->disp.screen == GRIDSELECT && app_p->game_state.defstats != STATS_DONE)   // Take UART input while in game or gridselect screen
    //     StatsInput(app_p, hal_p);
    // else if (app_p->disp.screen == GAME)
    //     GameInput(app_p, hal_p);


    // app_p->counter++;     // Update software counter every loop  
}
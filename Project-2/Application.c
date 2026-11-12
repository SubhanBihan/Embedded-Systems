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
            HSScreen(hal_p, &app_p->disp, &app_p->game_state);
            break;
        
        case GAME:
            app_p->game_state.new_game = true;
            GameScreen(hal_p, &app_p->disp, &app_p->game_state);
            break;

        case RESUME:
            GameScreen(hal_p, &app_p->disp, &app_p->game_state);
            break;
        
        case GAMEOVER:
            GameOverScreen(hal_p, &app_p->disp, &app_p->game_state);
            break;
        
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


    // Set high scores to 0
    for (i = 0; i < NUM_SCORES; i++) app_p->game_state.high_scores[i] = 0;

}


void Application_loop(HAL* hal_p, Application* app_p) {
    ScreenSelect(hal_p, app_p);
}
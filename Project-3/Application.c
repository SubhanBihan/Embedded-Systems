#include "Application.h"

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
        
        case GAME:
            GameScreen(hal_p, &app_p->disp, &app_p->gs);
            break;
        
        default:
            break;
    }
}

void Application_construct(Application* app_p) {
    // uint8_t i;

    // // Set the display struct
    // app_p->disp.gfx = GFX_construct(TEXT_LIT_COLOR, BACKGROUND_COLOR);

    app_p->disp.screen_changed = true;
    app_p->disp.redraw = true;
    app_p->disp.screen = TITLE;

}


void Application_loop(HAL* hal_p, Application* app_p) {
    ScreenSelect(hal_p, app_p);
}
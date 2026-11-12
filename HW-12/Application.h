#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "Display.h"

struct _Application {
  game_state_t game_state;
  display_t disp;
};

typedef struct _Application Application;

/**
 * Selects screen (Main, Game, HS, etc.)
 * @param app_p: Application struct object Pointer 
 * @param hal_p: HAL struct object Pointer 
 */
void ScreenSelect(HAL* hal_p, Application* app_p);

void Application_construct(Application* app_p);

/**
 * The main super-loop function of the application. We place this inside of a
 * single infinite loop in main. In this way, we can model a polling system of
 * FSMs. Every cycle of this loop function, we poll each of the FSMs one time,
 * followed by refreshing all inputs to the system through a convenient
 * [HAL_refresh()] call.
 *
 * @param app_p:  A pointer to the main Application object.
 * @param hal_p:  A pointer to the main HAL object
 */
void Application_loop(HAL* hal_p, Application* app_p);

#endif /* APPLICATION_H_ */
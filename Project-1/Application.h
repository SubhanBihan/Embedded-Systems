/*
 * Application.h
 *
 *  Created on: Dec 29, 2019
 *      Author: Matthew Zhong
 *  Supervisor: Leyla Nazhand-Ali
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

// #include <HAL/HAL.h>
#include <Display.h>

struct _Application {
  // Put your application members and FSM state variables here!
  // =========================================================================
  UART_Baudrate baudChoice;
  bool firstCall;
  game_state_t game_state;
  display_t disp;
  uint32_t counter;
};

typedef struct _Application Application;

// Called only a single time - inside of main(), where the application is
// constructed
Application Application_construct();

// Called once per super-loop of the main application.
void Application_loop(Graphics_Context* g_sContext_p, Application* app, HAL* hal);

// Called whenever the UART module needs to be updated
void Application_updateCommunications(Application* app, HAL* hal);

// Interprets an incoming character and echoes back to terminal what kind of
// character was received (number, letter, or other)
char Application_interpretIncomingChar(char);

void ScreenSelect(Graphics_Context* g_sContext_p, Application* app_p, HAL* hal_p);

void StatsInput(Application* app_p, HAL* hal_p);
void GameInput(Application* app_p, HAL* hal_p);

void UART_handleBackspace(HAL* hal_p, unsigned* count);

#endif /* APPLICATION_H_ */

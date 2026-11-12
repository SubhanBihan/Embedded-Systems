/*
 * API.c
 *
 *  Created on: Dec 29, 2019
 *      Author: Matthew Zhong
 *  Supervisor: Leyla Nazhand-Ali
 */

#include "HAL/HAL.h"

/**
 * Constructs a new API object. The API constructor should simply call the
 * constructors of each of its sub-members with the proper inputs.
 *
 * @return a properly constructed API object.
 */
void HAL_construct(HAL* hal_p) {
//   // The API object which will be returned at the end of construction
//   HAL hal;

  // Initialize buttons struct
  hal_p->buttons.LB1tapped = false;
  hal_p->buttons.LB2tapped = false;
  hal_p->buttons.BB1tapped = false;
  hal_p->buttons.BB2tapped = false;
  hal_p->buttons.JSBtapped = false;
  hal_p->buttons.AnyButtonTap = false;

  // Initialize the ADC and Joystick
  initADC();
  initJoyStick(&hal_p->JS);
  startADC();

//   // Construct the UART module inside of this HAL struct
//   hal.uart = UART_construct(USB_UART_INSTANCE, USB_UART_PORT, USB_UART_PINS);

//   // Enable the UART at 9600 BPS
//   // Call UART_SetBaud_Enable to achieve the above goal
//   UART_SetBaud_Enable(&hal.uart, BAUD_9600);

  // Construct the GFX module inside of this HAL struct
  hal_p->gfx = GFX_construct(FG_COLOR, BG_COLOR);

//   // Once we have finished building the API, return the completed struct.
//   return hal;
}

/**
 * Upon every new cycle of the main super-loop, we MUST UPDATE the status of
 * all inputs. In this program, this function is called only once in the
 * Application_loop() function. Since the Application_loop() function is called
 * once per loop of the while (true) loop in main, we are effectively polling
 * all inputs once per loop.
 *
 * @param hal:  The API whose input modules we wish to refresh
 */
void HAL_refresh(HAL* hal_p) {
  // Refresh buttons
  updateButtons(&hal_p->buttons);

  // Refresh Joystick
  JoyStick_refresh(&hal_p->JS);

  // Not real TODO: No need to add anything for UART
}

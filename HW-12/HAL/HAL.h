/*
 * HAL.h
 *
 *  Created on: Dec 29, 2019
 *      Author: Matthew Zhong
 *  Supervisor: Leyla Nazhand-Ali
 */

#ifndef HAL_HAL_H_
#define HAL_HAL_H_

// #include "HAL/Button.h"
#include "HAL/HAL_ADC.h"
#include "HAL/Button.h"
#include "HAL/Graphics.h"
#include "HAL/Buzzer.h"
#include "HAL/TimerA.h"
// #include <HAL/UART.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/**============================================================================
 * The main HAL struct. This struct encapsulates all of the other input structs
 * in this application as individual members. This includes all LEDs, all
 * Buttons, one HWTimer from which all software timers should reference, the
 * Joystick, and any other peripherals with which you wish to interface.
 * ============================================================================
 * USAGE WARNINGS
 * ============================================================================
 * YOU SHOULD HAVE EXACTLY ONE HAL STRUCT IN YOUR ENTIRE PROJECT. We recommend
 * you put this struct inside of a main [Application] object so that every
 * single function in your application has access to the main inputs and outputs
 * which interface with the hardware on the MSP432.
 */
struct _HAL {
  // Buttons
  buttons_t buttons;

  // Joystick
  JoyStick JS;

  // // UART - Construct a new UART instance
  // UART uart;

  //GFX - Construct a new GFX instance
  GFX gfx;
};
typedef struct _HAL HAL;

// Constructs an HAL object by calling the constructor of each individual member
void HAL_construct(HAL* hal_p);

// Refreshes all necessary inputs in the HAL
void HAL_refresh(HAL* hal_p);

#endif /* HAL_HAL_H_ */

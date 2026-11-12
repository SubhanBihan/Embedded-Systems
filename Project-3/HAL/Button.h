/*
 * Button.h
 *
 *  Created on: Apr 6, 2021
 *      Author: leyla
 */

#ifndef HAL_BUTTON_H_
#define HAL_BUTTON_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "HAL/LED.h"
#include "HAL/Timer.h"

#define LAUNCHPAD_S1_PORT GPIO_PORT_P1
#define LAUNCHPAD_S1_PIN GPIO_PIN1

#define LAUNCHPAD_S2_PORT GPIO_PORT_P1
#define LAUNCHPAD_S2_PIN GPIO_PIN4

#define BOOSTERPACK_S1_PORT GPIO_PORT_P5
#define BOOSTERPACK_S1_PIN GPIO_PIN1

#define BOOSTERPACK_S2_PORT GPIO_PORT_P3
#define BOOSTERPACK_S2_PIN GPIO_PIN5

#define BOOSTERPACK_JS_PORT GPIO_PORT_P4
#define BOOSTERPACK_JS_PIN GPIO_PIN1

// This structure holds the tapping status of all the buttons in our kit
typedef struct {
  bool LB1tapped;
  bool LB2tapped;
  bool BB1tapped;
  bool BB2tapped;
  bool JSBtapped;
  // bool AnyButtonTap;
} buttons_t;

// This function initializes all buttons
void initButtons();

// This function updates the tapping status of all the buttons
void updateButtons(buttons_t* buttons);

// Check button tap status and

#endif /* HAL_BUTTON_H_ */

/*
 * HAL.h
 *
 *  Created on: Dec 29, 2019
 *      Author: Matthew Zhong
 */

#ifndef HAL_HAL_H_
#define HAL_HAL_H_

#include <HAL/Button.h>
#include <HAL/LED.h>
#include <HAL/Timer.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/**============================================================================
 * The main HAL struct. This struct encapsulates all of the other input structs
 * in this application as individual members. This includes all LEDs, all
 * Buttons, one HWTimer from which all software timers should reference, the
 * Joystick, and any other peripherals with which you wish to interface.
 * ============================================================================
 * USAGE WARNINGS
 * ============================================================================
 * YOU SHOULD HAVE EXACTLY ONE HAL STRUCT IN YOUR ENTIRE PROJECT.
 */
struct _HAL
{
    // LEDs - Left Launchpad LED (the LED is only a single color - red)
    LED launchpadLED1;

    // LEDs - Right Launchpad LEDs (one LED struct for red, one for blue, etc.)
    LED launchpadLED2Red;
    LED launchpadLED2Blue;
    LED launchpadLED2Green;

    // LEDs - Boosterpack
    LED BoosterpackLEDRed;
    LED BoosterpackLEDBlue;
    LED BoosterpackLEDGreen;

    // Buttons - Launchpad S1 and S2
    Button launchpadS1;
    Button launchpadS2;

    // Buttons - Boosterpack S1 and S2
    Button BoosterpackS1;
    Button BoosterpackS2;
};

typedef struct _HAL HAL;

// Constructs an HAL object by calling the constructor of each individual member
HAL HAL_construct();

// Refreshes all necessary inputs in the HAL
void HAL_refresh(HAL* hal_p);

#endif /* HAL_HAL_H_ */

/*
 * API.c
 *
 *  Created on: Dec 29, 2019
 *      Author: Matthew Zhong
 */

#include <HAL/HAL.h>

/**
 * Constructs a new API object. The API constructor should simply call the constructors of each
 * of its sub-members with the proper inputs.
 *
 * @return a properly constructed API object.
 */
HAL HAL_construct()
{
    // The API object which will be returned at the end of construction
    HAL hal;

    // Initialize all LEDs by calling their constructors with correctly-defined arguments.
    hal.launchpadLED1 = LED_construct(LAUNCHPAD_LED1_PORT, LAUNCHPAD_LED1_PIN);

    hal.launchpadLED2Red   = LED_construct(LAUNCHPAD_LED2_RED_PORT  , LAUNCHPAD_LED2_RED_PIN  );
    hal.launchpadLED2Green = LED_construct(LAUNCHPAD_LED2_GREEN_PORT, LAUNCHPAD_LED2_GREEN_PIN);
    hal.launchpadLED2Blue  = LED_construct(LAUNCHPAD_LED2_BLUE_PORT , LAUNCHPAD_LED2_BLUE_PIN );

    hal.BoosterpackLEDRed   = LED_construct(BOOSTERPACK_LED_RED_PORT  , BOOSTERPACK_LED_RED_PIN  );
    hal.BoosterpackLEDGreen = LED_construct(BOOSTERPACK_LED_GREEN_PORT, BOOSTERPACK_LED_GREEN_PIN);
    hal.BoosterpackLEDBlue  = LED_construct(BOOSTERPACK_LED_BLUE_PORT , BOOSTERPACK_LED_BLUE_PIN );

    // Initialize all Buttons by calling their constructors with correctly-defined arguments.
    hal.launchpadS1 = Button_construct(LAUNCHPAD_S1_PORT, LAUNCHPAD_S1_PIN);        // Launchpad S1
    hal.launchpadS2 = Button_construct(LAUNCHPAD_S2_PORT, LAUNCHPAD_S2_PIN);        // Launchpad S2

    hal.BoosterpackS1 = Button_construct(BOOSTERPACK_S1_PORT, BOOSTERPACK_S1_PIN);        // Boosterpack S1
    hal.BoosterpackS2 = Button_construct(BOOSTERPACK_S2_PORT, BOOSTERPACK_S2_PIN);        // Boosterpack S2

    // Once we have finished building the API, return the completed struct.
    return hal;
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
void HAL_refresh(HAL* hal_p)
{
    // Refresh Launchpad buttons
    Button_refresh(&hal_p->launchpadS1);
    Button_refresh(&hal_p->launchpadS2);

    // Refresh Boosterpack buttons
    Button_refresh(&hal_p->BoosterpackS1);
    Button_refresh(&hal_p->BoosterpackS2);

}

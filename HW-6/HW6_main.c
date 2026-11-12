// Fall 2025 - HW6 - Problem 1

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

/* HAL and Application includes */
#include <Application.h>
#include <HAL/HAL.h>
#include <HAL/Timer.h>


/**
 * The main entry point of your project. The main function should immediately
 * stop the Watchdog timer, call the Application constructor, and then
 * repeatedly call the main super-loop function. The Application constructor
 * should be responsible for initializing all hardware components as well as all
 * other finite state machines you choose to use in this project.
 *
 * THIS FUNCTION IS ALREADY COMPLETE. Unless you want to temporarily experiment
 * with some behavior of a code snippet you may have, we DO NOT RECOMMEND
 * modifying this function in any way.
 */
int main(void)
{
    // Stop Watchdog Timer - THIS SHOULD ALWAYS BE THE FIRST LINE OF YOUR MAIN
    WDT_A_holdTimer();

    // Initialize the system clock and background hardware timer, used to enable
    // software timers to time their measurements properly.
    InitSystemTiming();

    // Initialize the main Application object and HAL object
    HAL hal = HAL_construct();
    Application app = Application_construct();

    // Main super-loop! In a polling architecture, this function should call
    // your main FSM function over and over.
    while (true)
    {
        HAL_refresh(&hal);
        Application_loop(&app, &hal);
    }
}

/**
 * The main constructor for your application. This function should initialize
 * each of the FSMs which implement the application logic of your project.
 *
 * @return a completely initialized Application object
 */
Application Application_construct()
{
    Application app;

    app.smallFSMstate = Green;
    app.boosterFSMState = B_Off;
    return app;
}

/**
 * The main super-loop function of the application. We place this inside of a
 * single infinite loop in main. In this way, we can model a polling system of
 * FSMs. Every cycle of this loop function, we poll each of the FSMs one time,
 * followed by refreshing all inputs to the system through a convenient
 * [HAL_refresh()] call.
 *
 * @param app:  A pointer to the main Application object.
 * @param hal:  A pointer to the main HAL object
 */
void Application_loop(Application* app_p, HAL* hal_p)
{
    // Turn on the LED1 on Launchpad on whenever the Launchpad S1 is pressed
    // This is a simple logic which has no memory
    // Use the below snippet to test part A and B
    // Comment this snippet when done with part A and B
    // if (Button_isPressed(&hal_p->BoosterpackS1))
    // {
    //     LED_turnOn(&hal_p->BoosterpackLEDRed);
    // }
    // else
    // {
    //     LED_turnOff(&hal_p->BoosterpackLEDRed);
    // }
    // End of part A and B testing

    // Toggle the LED1 on Launchpad whenever the Launchpad S1 goes from released to pressed (tapped)
    // This logic has memory and needs FSM for its implementation
    // Use the below snippet to test part C
//    if (Button_isTapped(&hal_p->BoosterpackS1))
//    {
//        LED_toggle(&hal_p->launchpadLED1);
//    }
    // End of part C testing


    // Turn the Launchapd RGB LED into various colors
    // Use the below line to test this function. Change RED to various colors, one at a time
    // The below line is for testing part D
//    turnLaunchpadRGB(hal_p, RED);
    // End of part D testing


    // Turn the Boosterpack RGB LED into various colors
    // Use the below line to test this function. Change RED to various colors, one at a time
    // The below line is for testing part E
//    turnBoosterpackRGB(hal_p, RED);
    // End of part E testing

    // Run the small FSM and take action with the Launchpad RGB based on its output
    // The below snippet is the inspiration for part E, so that you can observe the working of FSM before your implement yours
//    _RGBstatus RGBstatus = smallFSM(app_p, hal_p);
//    turnLaunchpadRGB(hal_p, RGBstatus);

//    RGBstatus = HW6FSM(app_p, hal_p);
//    turnBoosterpackRGB(hal_p, RGBstatus);

    // End of inspiration for part F

}

_RGBstatus smallFSM(Application* app_p, HAL* hal_p) {
    _appSmallFSMstate state = app_p->smallFSMstate; //This is not needed. We could work with app->mainFSMstate directly, but that is too much typing!

    // output
    _RGBstatus RGBstatus;

    switch (state) {
    case Green:
        if (hal_p->launchpadS1.isTapped)
            state = GreenToRed; // next state

        // The output of this state is GREEN regardless of the input
        RGBstatus = GREEN;
        break;

    case GreenToRed:
        if (hal_p->launchpadS1.isTapped) {
            state = Red;  // next state
            RGBstatus = RED;
        }
        else if (hal_p->launchpadS2.isTapped) {
            state = Green;  // next state
            RGBstatus = GREEN;
        } else
            RGBstatus = GREEN; //state is not changing. So, no need to type that in.
        break;

    case Red:
        if (hal_p->launchpadS2.isTapped)
            state = RedToGreen; // next state

        // The output of this state is GREEN regardless of the input
        RGBstatus = RED;
        break;

    case RedToGreen:
        if (hal_p->launchpadS2.isTapped) {
            state = Green;  // next state
            RGBstatus = GREEN;
        }
        else if (hal_p->launchpadS1.isTapped) {
            state = Red;  // next state
            RGBstatus = RED;
        } else
          RGBstatus = RED; //state is not changing. So, no need to type that in.

    }

    // Important! Don't forget to update app_p->smallFSMstate with the new state.
    app_p->smallFSMstate = state;
    return RGBstatus;
}

_RGBstatus HW6FSM(Application* app_p, HAL* hal_p) {
    _appBoosterFSMState state = app_p->boosterFSMState;

    // output
    _RGBstatus RGBstatus;

    switch (state) {
        case B_Off:
            if (hal_p->BoosterpackS1.isTapped)
                state = BB1_Once; // next state
            else if (hal_p->BoosterpackS2.isTapped)
                state = BB2_Once; // next state

            break;
        case BB1_Once:
            if (hal_p->BoosterpackS1.isTapped) {
                state = B_Blue; // next state
                RGBstatus = BLUE;
            }
            else if (hal_p->BoosterpackS2.isTapped) {
                state = B_Red; // next state
                RGBstatus = RED;
            }

            break;
        case BB2_Once:
            if (hal_p->BoosterpackS1.isTapped) {
                state = B_Yellow; // next state
                RGBstatus = YELLOW;
            }
            else if (hal_p->BoosterpackS2.isTapped) {
                state = B_Green; // next state
                RGBstatus = GREEN;
            }

            break;
        
        case B_Blue:
            if (hal_p->BoosterpackS1.isTapped) {
                state = B_Cyan; // next state
                RGBstatus = CYAN;
            }
            else if (hal_p->BoosterpackS2.isTapped) {
                state = B_Off; // next state
                RGBstatus = OFF;
            }

            break;
        
        case B_Green:
            if (hal_p->BoosterpackS1.isTapped) {
                state = B_Off; // next state
                RGBstatus = OFF;
            }
            else if (hal_p->BoosterpackS2.isTapped) {
                state = B_Magenta; // next state
                RGBstatus = MAGENTA;
            }

            break;
        
        case B_Cyan:
        case B_Yellow:
        case B_Red:
        case B_Magenta:
            if (hal_p->BoosterpackS1.isTapped || hal_p->BoosterpackS2.isTapped) {
                state = B_Off; // next state
                RGBstatus = OFF;
            }

            break;
    }


    app_p->boosterFSMState = state;
    return RGBstatus;
}


// This function is not complete. It does not support all _RGBstatus enumerations.
void turnLaunchpadRGB(HAL* hal_p, _RGBstatus RGBstatus) {
    switch(RGBstatus)
    {
    case OFF:
        LED_turnOff(&hal_p->launchpadLED2Blue);
        LED_turnOff(&hal_p->launchpadLED2Red);
        LED_turnOff(&hal_p->launchpadLED2Green);
        break;
    case GREEN:
        LED_turnOff(&hal_p->launchpadLED2Blue);
        LED_turnOff(&hal_p->launchpadLED2Red);
        LED_turnOn(&hal_p->launchpadLED2Green);
        break;
    case BLUE:
        LED_turnOn(&hal_p->launchpadLED2Blue);
        LED_turnOff(&hal_p->launchpadLED2Red);
        LED_turnOff(&hal_p->launchpadLED2Green);
        break;
    case RED:
        LED_turnOff(&hal_p->launchpadLED2Blue);
        LED_turnOn(&hal_p->launchpadLED2Red);
        LED_turnOff(&hal_p->launchpadLED2Green);
        break;
    case WHITE:
        LED_turnOn(&hal_p->launchpadLED2Blue);
        LED_turnOn(&hal_p->launchpadLED2Red);
        LED_turnOn(&hal_p->launchpadLED2Green);
        break;
    case CYAN:
        LED_turnOn(&hal_p->launchpadLED2Blue);
        LED_turnOff(&hal_p->launchpadLED2Red);
        LED_turnOn(&hal_p->launchpadLED2Green);
        break;
    case MAGENTA:
        LED_turnOn(&hal_p->launchpadLED2Blue);
        LED_turnOn(&hal_p->launchpadLED2Red);
        LED_turnOff(&hal_p->launchpadLED2Green);
        break;
    case YELLOW:
        LED_turnOff(&hal_p->launchpadLED2Blue);
        LED_turnOn(&hal_p->launchpadLED2Red);
        LED_turnOn(&hal_p->launchpadLED2Green);
        break;
    }
}

void turnBoosterpackRGB(HAL* hal_p, _RGBstatus RGBstatus) {
    switch(RGBstatus)
    {
    case OFF:
        LED_turnOff(&hal_p->BoosterpackLEDBlue);
        LED_turnOff(&hal_p->BoosterpackLEDRed);
        LED_turnOff(&hal_p->BoosterpackLEDGreen);
        break;
    case GREEN:
        LED_turnOff(&hal_p->BoosterpackLEDBlue);
        LED_turnOff(&hal_p->BoosterpackLEDRed);
        LED_turnOn(&hal_p->BoosterpackLEDGreen);
        break;
    case BLUE:
        LED_turnOn(&hal_p->BoosterpackLEDBlue);
        LED_turnOff(&hal_p->BoosterpackLEDRed);
        LED_turnOff(&hal_p->BoosterpackLEDGreen);
        break;
    case RED:
        LED_turnOff(&hal_p->BoosterpackLEDBlue);
        LED_turnOn(&hal_p->BoosterpackLEDRed);
        LED_turnOff(&hal_p->BoosterpackLEDGreen);
        break;
    case WHITE:
        LED_turnOn(&hal_p->BoosterpackLEDBlue);
        LED_turnOn(&hal_p->BoosterpackLEDRed);
        LED_turnOn(&hal_p->BoosterpackLEDGreen);
        break;
    case CYAN:
        LED_turnOn(&hal_p->BoosterpackLEDBlue);
        LED_turnOff(&hal_p->BoosterpackLEDRed);
        LED_turnOn(&hal_p->BoosterpackLEDGreen);
        break;
    case MAGENTA:
        LED_turnOn(&hal_p->BoosterpackLEDBlue);
        LED_turnOn(&hal_p->BoosterpackLEDRed);
        LED_turnOff(&hal_p->BoosterpackLEDGreen);
        break;
    case YELLOW:
        LED_turnOff(&hal_p->BoosterpackLEDBlue);
        LED_turnOn(&hal_p->BoosterpackLEDRed);
        LED_turnOn(&hal_p->BoosterpackLEDGreen);
        break;
    }
}



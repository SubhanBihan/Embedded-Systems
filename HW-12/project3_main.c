// Starter code for Project 3. Good luck!

/* TI includes */
#include <LcdDriver/Crystalfontz128x128_ST7735.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>

/* Standard includes */
#include <stdio.h>

#include "Application.h"

void initialize();
void main_loop(HAL* hal_p, Application* app_p);
void sleep();

int main(void) {
    initialize();

    // Initialize the main Application object and HAL object
    HAL hal;
    HAL_construct(&hal);
    Application app;
    Application_construct(&app);

    while (1) {
        sleep();

        main_loop(&hal, &app);
    }
}

void sleep() {
    // The Launchpad Green LED is used to signify the processor is in low-power
    // mode. From the human perspective, it should seem the processor is always
    // asleep except for fractions of second here and there.

    TurnOn_LLG();
    // Enters the Low Power Mode 0 - the processor is asleep and only responds to
    // interrupts
    PCM_gotoLPM0();
    TurnOff_LLG();
}

void main_loop(HAL* hal_p, Application* app_p) {
    // // The first step is update the tapping status of all buttons
    // buttons_t buttons = updateButtons();

    // TODO: You probably want to define a state variable and an enum associated
    // with it You can then control the state of your application based on the
    // hardware inputs Refer to the Guess-the-color example.

    HAL_refresh(hal_p);
    Application_loop(hal_p, app_p);
}

// Initialization part is always device dependent and therefore does not change
// much with HAL
void initialize() {
    // Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);
    InitSystemTiming();

    initLEDs();
    initButtons();
    // TODO: Add other initializations here
    Buzzer_Init();
}

/*
 * button.c
 *
 *  Created on: Apr 6, 2021
 *      Author: leyla
 */

#include "HAL/Button.h"

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>


// A boolean variable that is true when a high-to-low transition is sensed on
// JSB For a global variable, the keyword static limits the scope of the
// variable to this function only. This means functions in other files of the
// project cannot access this variable
volatile static bool LB1modified;
volatile static bool LB2modified;
volatile static bool BB1modified;
volatile static bool BB2modified;
volatile static bool JSBmodified;

// An internal function that initializes a button and enables the high-to-low
// transition
void initButton(uint_fast8_t selectedPort, uint_fast16_t selectedPins) {
    // This sets up an input with pull-up resistor. If the input does not need the
    // pull-up resistor, the extra added resistor is harmless.
    GPIO_setAsInputPinWithPullUpResistor(selectedPort, selectedPins);

    // clear interrupt on port selectedPort, pin selectedPins
    // If we fail to this, we "might" get an interrupt as soon we enable interrupt
    GPIO_clearInterruptFlag(selectedPort, selectedPins);

    // enable interrupt on port selectedPort, pin selectedPins
    GPIO_enableInterrupt(selectedPort, selectedPins);

    // the interrupt is triggered on high to low transition (tapping)
    GPIO_interruptEdgeSelect(selectedPort, selectedPins,
                             GPIO_HIGH_TO_LOW_TRANSITION);
}

void initButtons() {
    initButton(LAUNCHPAD_S1_PORT, LAUNCHPAD_S1_PIN);  // LB1
    initButton(LAUNCHPAD_S2_PORT, LAUNCHPAD_S2_PIN);  // LB2
    initButton(BOOSTERPACK_S1_PORT, BOOSTERPACK_S1_PIN);  // BB1
    initButton(BOOSTERPACK_S2_PORT, BOOSTERPACK_S2_PIN);  // BB1
    initButton(BOOSTERPACK_JS_PORT, BOOSTERPACK_JS_PIN);  // JSB

    // enable the ports related to the buttons
    Interrupt_enableInterrupt(INT_PORT1);
    Interrupt_enableInterrupt(INT_PORT3);
    Interrupt_enableInterrupt(INT_PORT4);
    Interrupt_enableInterrupt(INT_PORT5);

    // This allows us to start from a clean slate
    LB1modified = false;
    LB2modified = false;
    BB1modified = false;
    BB2modified = false;
    JSBmodified = false;
}

void PORT1_IRQHandler() {
    // We check to see if the port1 interrupt came from LB1
    if (GPIO_getInterruptStatus(LAUNCHPAD_S1_PORT, LAUNCHPAD_S1_PIN)) {
        LB1modified = true;

        // A very critical step: If we don't clear the interrupt, the ISR will be
        // called again and again.
        GPIO_clearInterruptFlag(LAUNCHPAD_S1_PORT, LAUNCHPAD_S1_PIN);
    }

    if (GPIO_getInterruptStatus(LAUNCHPAD_S2_PORT, LAUNCHPAD_S2_PIN)) {
        LB2modified = true;
        GPIO_clearInterruptFlag(LAUNCHPAD_S2_PORT, LAUNCHPAD_S2_PIN);
    }
}

void PORT3_IRQHandler() {
    if (GPIO_getInterruptStatus(BOOSTERPACK_S2_PORT, BOOSTERPACK_S2_PIN)) {
        BB2modified = true;
        GPIO_clearInterruptFlag(BOOSTERPACK_S2_PORT, BOOSTERPACK_S2_PIN);
    }
}

void PORT4_IRQHandler() {
    if (GPIO_getInterruptStatus(BOOSTERPACK_JS_PORT, BOOSTERPACK_JS_PIN)) {
        JSBmodified = true;
        GPIO_clearInterruptFlag(BOOSTERPACK_JS_PORT, BOOSTERPACK_JS_PIN);
    }
}

void PORT5_IRQHandler() {
    if (GPIO_getInterruptStatus(BOOSTERPACK_S1_PORT, BOOSTERPACK_S1_PIN)) {
        BB1modified = true;
        GPIO_clearInterruptFlag(BOOSTERPACK_S1_PORT, BOOSTERPACK_S1_PIN);
    }
}

bool LB1tapped() {
    bool tapped = LB1modified;
    LB1modified = false;
    return tapped;
}

bool LB2tapped() {
    bool tapped = LB2modified;
    LB2modified = false;
    return tapped;
}

bool BB1tapped() {
    bool tapped = BB1modified;
    BB1modified = false;
    return tapped;
}

bool BB2tapped() {
    bool tapped = BB2modified;
    BB2modified = false;
    return tapped;
}

bool JSBtapped() {
    bool tapped = JSBmodified;
    JSBmodified = false;
    return tapped;
}
// This function calls all the functions that check button status and stores
// them in one structure This will allow the user to reliably get the latest
// button status. If we choose not to use this method, the user has to be
// careful to call JSBtapped() or any similar function only once in the main
// loop.
void updateButtons(buttons_t* buttons) {
    buttons->LB1tapped = LB1tapped();
    buttons->LB2tapped = LB2tapped();
    buttons->BB1tapped = BB1tapped();
    buttons->BB2tapped = BB2tapped();
    buttons->JSBtapped = JSBtapped();
    buttons->AnyButtonTap = buttons->LB1tapped || buttons->LB2tapped || buttons->BB1tapped || buttons->BB2tapped || buttons->JSBtapped;
}

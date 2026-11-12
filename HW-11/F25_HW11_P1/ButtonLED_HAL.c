/*
 *      Author: Leyla Nazhand-Ali
 */

#include "ButtonLED_HAL.h"

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// According to the schematics on page 37 of the Launchpad user guide,
// When a button is pressed, it is grounded (logic 0)
#define PRESSED 0
#define RELEASED 1

void initialize_LaunchpadLED1() {
  GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
}
void turnOn_LaunchpadLED1() {
  GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
}
void turnOff_LaunchpadLED1() {
  GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
}
void toggle_LaunchpadLED1() { GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0); }

void initialize_LaunchpadLED2_red() {
  GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
}
void turnOn_LaunchpadLED2_red() {
  GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
}
void turnOff_LaunchpadLED2_red() {
  GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
}
void toggle_LaunchpadLED2_red() {
  GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN0);
}

void initialize_LaunchpadLED2_green() {
  GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
}
void turnOn_LaunchpadLED2_green() {
  GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
}
void turnOff_LaunchpadLED2_green() {
  GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
}
void toggle_LaunchpadLED2_green() {
  GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN1);
}

void initialize_LaunchpadLED2_blue() {
  GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
}
void turnOn_LaunchpadLED2_blue() {
  GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
}
void turnOff_LaunchpadLED2_blue() {
  GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
}
void toggle_LaunchpadLED2_blue() {
  GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN2);
}

void initialize_BoosterpackLED_red() {
  GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN6);
}
void turnOn_BoosterpackLED_red() {
  GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN6);
}
void turnOff_BoosterpackLED_red() {
  GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
}
void toggle_BoosterpackLED_red() {
  GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN6);
}

void initialize_BoosterpackLED_green() {
  GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4);
}
void turnOn_BoosterpackLED_green() {
  GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4);
}
void turnOff_BoosterpackLED_green() {
  GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4);
}
void toggle_BoosterpackLED_green() {
  GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN4);
}

void initialize_BoosterpackLED_blue() {
  GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN6);
}
void turnOn_BoosterpackLED_blue() {
  GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN6);
}
void turnOff_BoosterpackLED_blue() {
  GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN6);
}
void toggle_BoosterpackLED_blue() {
  GPIO_toggleOutputOnPin(GPIO_PORT_P5, GPIO_PIN6);
}

void initialize_LaunchpadLeftButton() {
  GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
}
char switchStatus_LaunchpadLeft() {
  return (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1));
}
char LaunchpadLeftButton_pressed() {
  return ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)) == PRESSED);
}

void initialize_LaunchpadRightButton() {
  GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);
}
char switchStatus_LaunchpadRight() {
  return (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4));
}
char LaunchpadRightButton_pressed() {
  return ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4)) == PRESSED);
}

void initialize_BoosterpackTopButton() {
  GPIO_setAsInputPin(GPIO_PORT_P5, GPIO_PIN1);
}
char switchStatus_BoosterpackTop() {
  return (GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN1));
}
char BoosterpackTopButton_pressed() {
  return ((GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN1)) == PRESSED);
}

void initialize_BoosterpackBottomButton() {
  GPIO_setAsInputPin(GPIO_PORT_P3, GPIO_PIN5);
}
char switchStatus_BoosterpackBottom() {
  return (GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN5));
}
char BoosterpackBottomButton_pressed() {
  return ((GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN5)) == PRESSED);
}

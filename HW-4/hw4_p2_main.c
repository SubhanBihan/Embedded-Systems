// Fall 2025 - HW4 - Problem 2

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

//enum for your use
typedef enum { MAGENTA, RED, BLUE, YELLOW, OFF } color_t;

// According to the schematics on page 37 of the Launchpad user guide,
// When a button is pressed, it is grounded (logic 0)
#define PRESSED 0

// void TurnOn_Launchpad_LL1();
// void TurnOff_Launchpad_LL1();
void TurnOn_BLR();
void TurnOff_BLR();
void TurnOn_BLB();
void TurnOff_BLB();
void TurnOn_BLG();
void TurnOff_BLG();
// char LB2isPressed();
char BB1isPressed();
char BB2isPressed();
void Turn_BoosterPack_LED(color_t desiredColor);

// This function initializes all the peripherals
void initialize();

// In this example, the high-level function, main, is readable.
// It is also easy to port this function to other platforms with similar
// peripherals such as an LED and a switch
int main(void) {
  // initialize all the peripherals
  initialize();

  while (1) {
    Turn_BoosterPack_LED((BB2isPressed() << 1) + BB1isPressed());   // Input is 2 bits: {BB2_pressed, BB1_pressed}
  }
}

// The HAL itself is written using Driverlib, so it is much easier to implement.
void initialize() {
  // step 1: Stop watchdog timer
  // We do this at the beginning of all our programs for now.Later we learn more
  // about it.
  WDT_A_hold(WDT_A_BASE);

  // RGB LED channels
  GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN6);
  GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN6);
  GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4);

  // BB1 and BB2
  GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN1);
  GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN5);
}

// void TurnOn_Launchpad_LL1() {
//   GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
// }

// void TurnOff_Launchpad_LL1() {
//   GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
// }

void TurnOn_BLR() {
  GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN6);
}

void TurnOff_BLR() {
  GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
}

void TurnOn_BLB() {
  GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN6);
}

void TurnOff_BLB() {
  GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN6);
}

void TurnOn_BLG() {
  GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4);
}

void TurnOff_BLG() {
  GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4);
}

// char LB2isPressed() {
//   return ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4) == PRESSED));
// }

char BB1isPressed() {
  return ((GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN1) == PRESSED));
}

char BB2isPressed() {
  return ((GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN5) == PRESSED));
}


void Turn_BoosterPack_LED(color_t desiredColor){
    switch(desiredColor){
      //check the switch documentation here: https://www.geeksforgeeks.org/c-switch-statement/
      case MAGENTA:
          TurnOn_BLR();
          TurnOff_BLG();
          TurnOn_BLB();
          break;
      case RED:
          TurnOn_BLR();
          TurnOff_BLG();
          TurnOff_BLB();
          break;
      case BLUE:
          TurnOff_BLR();
          TurnOff_BLG();
          TurnOn_BLB();
          break;
      case YELLOW:
          TurnOn_BLR();
          TurnOn_BLG();
          TurnOff_BLB();
          break;
      default:
      case OFF:
          TurnOff_BLR();
          TurnOff_BLG();
          TurnOff_BLB();
          break;
    }
}

// Fall 2025 - HW4 - Problem 1
// This application uses buttons and LEDs
// The operation is such that if you press the left button, as long as you keep
// the button pressed, the LED is on. As soon as you release the button, LED is
// off and stays off until the button is pressed again. This example uses
// memory-mapped registers to communicate with GPIO peripherals. This example
// doesn't use any Driverlib function. However, it does build a Hardware
// Abstraction Layer (HAL). The main function is easy to read, but the HAL
// functions themselves are hard to write since we do not use Driverlib.

// According to the schematics on page 37 of the Launchpad user guide,
// Right button (button 2, S2) is connected to pin 4 (bit4)
#define LB2 (1 << 4)

#define BB2 (1 << 5)

// According to the schematics on page 37 of the Launchpad user guide,
// launchpad LED (LED 1) is connected to pin 0 (bit0)
#define LL1 (1 << 0)

#define BLR (1 << 6)

// According to the schematics on page 37 of the Launchpad user guide,
// When a button is pressed, it is grounded (logic 0)
#define PRESSED 0

void TurnOn_Launchpad_LL1();
void TurnOff_Launchpad_LL1();
char LB2isPressed();

void TurnOn_BLR();
void TurnOff_BLR();
char BB2isPressed();

// global variables are not recommended.
// However, without a Driver library, using global variables is a reasonable
// solution.
unsigned char *P1IN_pointer;
unsigned char *P1OUT_pointer;
unsigned char *P1REN_pointer;
unsigned char *P1DIR_pointer;

unsigned char *P2IN_pointer;
unsigned char *P2OUT_pointer;
unsigned char *P2REN_pointer;
unsigned char *P2DIR_pointer;

unsigned char *P3IN_pointer;
unsigned char *P3OUT_pointer;
unsigned char *P3REN_pointer;
unsigned char *P3DIR_pointer;

// This function initializes all the peripherals
void initialize();

// In this example, the high-level function, main, is readable.
// It is also easy to port this function to other platforms with similar
// peripherals such as an LED and a switch
int main(void) {
  // initialize all the peripherals
  initialize();

  while (1) {
    // If the button is not pressed, keep the LED off
    if (!LB2isPressed()) TurnOff_Launchpad_LL1();

    // otherwise, turn the LED on
    else
      TurnOn_Launchpad_LL1();

    // If the button is not pressed, keep the LED off
    if (!BB2isPressed()) TurnOff_BLR();

    // otherwise, turn the LED on
    else
      TurnOn_BLR();

  }


}

// The HAL itself is written without Driverlib, so it is very hard to implement.
void initialize() {
  P1IN_pointer = (unsigned char *)0x40004C00;
  P1OUT_pointer = (unsigned char *)0x40004C02;
  P1REN_pointer = (unsigned char *)0x40004C06;
  P1DIR_pointer = (unsigned char *)0x40004C04;

  // Initializing LL1
  // According to table 12.1 on page 678 of MSP432 User guide,
  // to create an output, all you need is to write a 1 to PxDIR for the specific
  // bit you want A common mistake is to write P1DIR = LL1 instead of P1DIR |=
  // LL1;
  *P1DIR_pointer |= LL1;  // Same as P1DIR = P1DIR | LL1;

  // Initializing RB1 (switch 2 or button 2 of Launchpad)
  // According to the table on page 678 of MSP432 User guide,
  // to create an input with pull-up resistor, you need to do three things

  // write a 0 to PxDIR for the specific bit you want
  *P1DIR_pointer &= ~LB2;

  // write a 1 to PxREN for the specific bit you want
  *P1REN_pointer |= LB2;

  // step 3.3: write a 1 to PxOUT for the specific bit you want
  *P1OUT_pointer |= LB2;  // select pull-up


  // FOR BB2
  P3IN_pointer = (unsigned char *)0x40004C20;
  P3OUT_pointer = (unsigned char *)0x40004C22;
  P3REN_pointer = (unsigned char *)0x40004C26;
  P3DIR_pointer = (unsigned char *)0x40004C24;

  // Initializing RB1 (switch 2 or button 2 of Launchpad)
  // According to the table on page 678 of MSP432 User guide,
  // to create an input with pull-up resistor, you need to do three things

  // write a 0 to PxDIR for the specific bit you want
  *P3DIR_pointer &= ~BB2;

  // write a 1 to PxREN for the specific bit you want
  *P3REN_pointer |= BB2;

  // step 3.3: write a 1 to PxOUT for the specific bit you want
  *P3OUT_pointer |= BB2;  // select pull-up


  // FOR RGB RED
  // P2IN_pointer = (unsigned char *)0x40004C00;
  P2OUT_pointer = (unsigned char *) 0x40004C03;
  // P2REN_pointer = (unsigned char *)0x40004C06;
  P2DIR_pointer = (unsigned char *)0x40004C05;

  // Initializing BLR
  // According to table 12.1 on page 678 of MSP432 User guide,
  // to create an output, all you need is to write a 1 to PxDIR for the specific
  // bit you want A common mistake is to write P2DIR = BLR instead of P2DIR |=
  // BLR;
  *P2DIR_pointer |= BLR;  // Same as P2DIR = P2DIR | BLR;
}

void TurnOn_Launchpad_LL1() { *P1OUT_pointer = *P1OUT_pointer | LL1; }

void TurnOff_Launchpad_LL1() { *P1OUT_pointer = *P1OUT_pointer & ~LL1; }

char LB2isPressed() { return ((*P1IN_pointer & LB2) == PRESSED); }

void TurnOn_BLR() { *P2OUT_pointer |= BLR; }

void TurnOff_BLR() { *P2OUT_pointer &= ~BLR; }

char BB2isPressed() { return ((*P3IN_pointer & BB2) == PRESSED); }


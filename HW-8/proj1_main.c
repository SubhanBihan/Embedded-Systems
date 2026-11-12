/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* HAL and Application includes */
#include <Application.h>
// #include <HAL/HAL.h>
#include <HAL/Timer.h>
#include <Display.h>


// Non-blocking check. Whenever Launchpad S1 is pressed, LED1 turns on.
static void InitNonBlockingLED() {
  GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
  GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
}

// Non-blocking check. Whenever Launchpad S1 is pressed, LED1 turns on.
static void PollNonBlockingLED() {
  GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
  if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == 0) {
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
  }
}

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

int main(void) {
  // Stop Watchdog Timer - THIS SHOULD ALWAYS BE THE FIRST LINE OF YOUR MAIN
  WDT_A_holdTimer();

  // Initialize the system clock and background hardware timer, used to enable
  // software timers to time their measurements properly.
  InitSystemTiming();

  // Initialize the main Application object and HAL object
  HAL hal = HAL_construct();
  Application app = Application_construct();

  // Do not remove this line. This is your non-blocking check.
  InitNonBlockingLED();

  // A structure that holds a large set of characteristics for graphics including foreground color, background color
  Graphics_Context g_sContext;

  Initialize(&g_sContext);

  // Main super-loop! In a polling architecture, this function should call
  // your main FSM function over and over.
  while (true) {
    // Do not remove this line. This is your non-blocking check.
    PollNonBlockingLED();
    HAL_refresh(&hal);
    Application_loop(&g_sContext, &app, &hal);
  }
}


/**
 * The main constructor for your application. This function should initialize
 * each of the FSMs which implement the application logic of your project.
 *
 * @return a completely initialized Application object
 */
Application Application_construct() {
  Application app;

  display_t disp = {true, true, MAIN, WHITE};
  
  game_state_t game_state;
  game_state.grid_length = 3;
  game_state.stats_change = true;
  game_state.new_cmd = true;
  game_state.cmd.action = NONE;
  game_state.cmd.sq_idx = -1;

  // Initialize local application state variables here!
  app.baudChoice = BAUD_9600;
  app.firstCall = true;
  app.disp = disp;
  app.game_state = game_state;

  return app;
}

/**
 * The main super-loop function of the application. We place this inside of a
 * single infinite loop in main. In this way, we can model a polling system of
 * FSMs. Every cycle of this loop function, we poll each of the FSMs one time,
 * followed by refreshing all inputs to the system through a convenient
 * [HAL_refresh()] call.
 *
 * @param app_p:  A pointer to the main Application object.
 * @param hal_p:  A pointer to the main HAL object
 */
void Application_loop(Graphics_Context* g_sContext_p, Application* app_p, HAL* hal_p) {
  // Restart/Update communications if first run or S2 is pressed
  if (Button_isTapped(&hal_p->launchpadS2) || app_p->firstCall) {
    Application_updateCommunications(app_p, hal_p);
  }

  switch (app_p->disp.screen)
  {
      case MAIN:
        MainScreen(g_sContext_p, hal_p, &app_p->disp);
        break;
      
      case INSTRUCTIONS:
        InstructionsScreen(g_sContext_p, hal_p, &app_p->disp);
        break;

      case HS:
        HSScreen(g_sContext_p, hal_p, &app_p->disp, 0);
        break;

      case GAME:
        GameScreen(g_sContext_p, hal_p, &app_p->disp, &app_p->game_state);
        break;

      // TODO: Game Over
      default:
        break;
  }

  static char collected[3];
  static unsigned count = 0, prev_count = 0;

  if (UART_hasChar(&hal_p->uart) && app_p->disp.screen == GAME) {
    char rxChar = UART_getChar(&hal_p->uart);
    char txChar = Application_interpretIncomingChar(rxChar);

    // Filtering logic for valid sequences: IG*, AP*, SP* (third char must be 1–8)
    bool accept = false;

    if (count == 0) {
      // Only accept I, A, or S as first letter
      if (txChar == 'I' || txChar == 'A' || txChar == 'S') {
        accept = true;
      }
    } else if (count == 1) {
      // Second character depends on first
      if ((collected[0] == 'I' && txChar == 'G') ||
          (collected[0] == 'A' && txChar == 'P') ||
          (collected[0] == 'S' && txChar == 'P')) {
        accept = true;
      }
    } else if (count == 2) {
      // Third character must be a digit 1–8
      if (txChar >= '1' && txChar <= '8') {
        accept = true;
      }
    }

    if (accept && count < 3 && UART_canSend(&hal_p->uart)) {
      UART_sendChar(&hal_p->uart, txChar);
      collected[count++] = txChar;
    }
    // Invalid chars are ignored and not echoed

    // Check for Enter key
    if (rxChar == '\r' || rxChar == '\n') {
      if (count > 0) {
        // ----- PROCESS COLLECTED CHARS -----
        if (count < 2) {
          // Action when less than 2 chars collected
          app_p->game_state.cmd.action = INVALID;
        } else {
          // Action using first two chars
          if (collected[0] == 'I' && collected[1] == 'G') {
            // Action for "IG"
            app_p->game_state.cmd.action = IG;
          }
          else if (collected[0] == 'A' && collected[1] == 'P') {
            // Action for "AP"
            app_p->game_state.cmd.action = AP;
          }
          else if (collected[0] == 'S' && collected[1] == 'P') {
            // Action for "SP"
            app_p->game_state.cmd.action = SP;
          }
          else {
            // Invalid or unrecognized prefix
            app_p->game_state.cmd.action = INVALID;
          }
        }

        if (count >= 3) {
          // Action using 3rd char
          // if (collected[2] >= '1' && collected[2] <= '8')
            app_p->game_state.cmd.sq_idx = get_square_idx(collected[2] - '0', app_p->game_state.grid_length);
          // else
          //   app_p->game_state.cmd.sq_idx = -1;
        }
        // -----------------------------------

        prev_count = count;
        count = 0;  // Wipe buffer
      }
      // return;  // Don’t echo Enter
    }
    

    if (app_p->game_state.cmd.action == INVALID || (app_p->game_state.event == CALM && (app_p->game_state.cmd.action == AP || app_p->game_state.cmd.action == SP) && prev_count < 3)) {
      UART_printMessage(hal_p, "\r\n\r\nInvalid Input. Try again!");
      // app_p->game_state.new_cmd = true;
      app_p->game_state.cmd.action = INVALID;
    }
  }
}



/**
 * Updates which LEDs are lit and what baud rate the UART module communicates
 * with, based on what the application's baud choice is at the time this
 * function is called.
 *
 * @param app_p:  A pointer to the main Application object.
 * @param hal_p:  A pointer to the main HAL object
 */
void Application_updateCommunications(Application* app_p, HAL* hal_p) {
  // When this application first loops, the proper LEDs aren't lit. The
  // firstCall flag is used to ensure that the
  if (app_p->firstCall) {
    app_p->firstCall = false;
  }

  // When BoosterPack S2 is tapped, circularly increment which baud rate is
  // used.
  else {
    uint32_t newBaudNumber =
        CircularIncrement((uint32_t)app_p->baudChoice, NUM_BAUD_CHOICES);
    app_p->baudChoice = (UART_Baudrate)newBaudNumber;
  }

  // Start/update the baud rate according to the one set above.
  UART_SetBaud_Enable(&hal_p->uart, app_p->baudChoice);

  // Based on the new application choice, turn on the correct LED.
  // To make your life easier, we recommend turning off all LEDs before
  // selectively turning back on only the LEDs that need to be relit.
  // -------------------------------------------------------------------------
  LED_turnOff(&hal_p->launchpadLED2Red);
  LED_turnOff(&hal_p->launchpadLED2Green);
  LED_turnOff(&hal_p->launchpadLED2Blue);

  // TODO: Turn on all appropriate LEDs according to the tasks below.
  switch (app_p->baudChoice) {
    // When the baud rate is 9600, turn on Launchpad LED Red
    case BAUD_9600:
      LED_turnOn(&hal_p->launchpadLED2Red);
      break;

    // TODO: When the baud rate is 19200, turn on Launchpad LED Green
    case BAUD_19200:
      LED_turnOn(&hal_p->launchpadLED2Green);
      break;

    // TODO: When the baud rate is 38400, turn on Launchpad LED Blue
    case BAUD_38400:
      LED_turnOn(&hal_p->launchpadLED2Blue);
      break;

    // TODO: When the baud rate is 57600, turn on all Launchpad LEDs
    // (illuminates white)
    case BAUD_57600:
      LED_turnOn(&hal_p->launchpadLED2Red);
      LED_turnOn(&hal_p->launchpadLED2Green);
      LED_turnOn(&hal_p->launchpadLED2Blue);
      break;

    // In the default case, this program will do nothing.
    default:
      break;
  }
}

// /**
//  * Interprets a character which was incoming and returns an interpretation of
//  * that character. If the input character is a letter, it return L for Letter,
//  * if a number return N for Number, and if something else, it return O for
//  * Other.
//  *
//  * @param rxChar: Input character
//  * @return :  Output character
//  */
// char Application_interpretIncomingChar(char rxChar) {
//   // The character to return back to sender. By default, we assume the letter
//   // to send back is an 'O' (assume the character is an "other" character)
//   char txChar = 'O';

//   // Numbers - if the character entered was a number, transfer back an 'N'
//   if (rxChar >= '0' && rxChar <= '9') {
//     txChar = 'N';
//   }

//   // Letters - if the character entered was a letter, transfer back an 'L'
//   if ((rxChar >= 'a' && rxChar <= 'z') || (rxChar >= 'A' && rxChar <= 'Z')) {
//     txChar = 'L';
//   }

//   return (txChar);
// }

/**
 * Converts any received letter to uppercase and returns it unchanged otherwise.
 *
 * @param rxChar: Input character
 * @return : Output character (uppercase if letter)
 */
char Application_interpretIncomingChar(char rxChar) {
  // If it's a lowercase letter, convert to uppercase
  if (rxChar >= 'a' && rxChar <= 'z') {
    rxChar = rxChar - 'a' + 'A';
  }

  // Return the (possibly modified) character
  return rxChar;
}


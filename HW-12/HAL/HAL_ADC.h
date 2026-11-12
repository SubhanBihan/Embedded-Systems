#ifndef HAL_ADC_H_
#define HAL_ADC_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
// #include "HAL/Button.h"

#define JOYSTICK_X_CENTER 7900
#define JOYSTICK_Y_CENTER 8300
#define JOYSTICK_X_MIN    1300
#define JOYSTICK_Y_MIN    3900
#define JOYSTICK_X_MAX    13900
#define JOYSTICK_Y_MAX    14900
#define LEFT_THRESHOLD  (JOYSTICK_X_CENTER - (JOYSTICK_X_CENTER - JOYSTICK_X_MIN) / 2)
#define RIGHT_THRESHOLD (JOYSTICK_X_CENTER + (JOYSTICK_X_MAX - JOYSTICK_X_CENTER) / 2)
#define DOWN_THRESHOLD  (JOYSTICK_Y_CENTER - (JOYSTICK_Y_CENTER - JOYSTICK_Y_MIN) / 2)
#define UP_THRESHOLD    (JOYSTICK_Y_CENTER + (JOYSTICK_Y_MAX - JOYSTICK_Y_CENTER) / 2)

// typedef enum {PRESSED_LEFT = -1, PRESSED_CENTERX, PRESSED_RIGHT} FSM_StateX_t;
// typedef enum {PRESSED_DOWN = -1, PRESSED_CENTERY, PRESSED_UP}    FSM_StateY_t;

typedef enum {LEFT = -1, CENTERX, RIGHT} DirectionX_t;
typedef enum {DOWN = -1, CENTERY, UP}    DirectionY_t;

typedef struct {
  // unsigned vx, vy;

  // Keeps track of FSM progress in the Debouncing FSM
  DirectionX_t StateX;
  DirectionY_t StateY;

  // // The timer which is used to wait out a bouncy input
  // SWTimer timerX, timerY;

  // The outputs of the FSM.
  DirectionX_t PushedX, TappedX;
  DirectionY_t PushedY, TappedY;
} JoyStick;

void initADC();
void startADC();
void initJoyStick(JoyStick* JS);
void getSampleJoyStick(unsigned *X, unsigned *Y);
void JoyStick_refresh(JoyStick* JS);

#endif

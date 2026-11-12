#ifndef HAL_ADC_H_
#define HAL_ADC_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

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

void initADC();
void startADC();
void initJoyStick();
void getSampleJoyStick(unsigned *X, unsigned *Y);

#endif

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "graphics_HAL.h"

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

typedef enum {MINUS = -1, ZERO, PLUS} direction_t;

// This function initializes all the peripherals except graphics
void initialize();
// void ModifyLEDColor(direction_t leftButtonWasPushed, direction_t rightButtonWasPushed);

void initADC();
void startADC();
void initJoyStick();
void getSampleJoyStick(unsigned *X, unsigned *Y);

void MoveCircle(Graphics_Context *g_sContext_p, direction_t joyStickPushed_X, direction_t joyStickPushed_Y);

int main(void)
{

    Graphics_Context g_sContext;

    initialize();
    InitGraphics(&g_sContext);
    draw_Base(&g_sContext);

    unsigned vx, vy;
    direction_t joyStickPushed_X = ZERO;
    direction_t joyStickPushed_Y = ZERO;

    while (1)
    {

        getSampleJoyStick(&vx, &vy);
        drawXY(&g_sContext, vx, vy);

        joyStickPushed_X = vx < LEFT_THRESHOLD ? MINUS : vx > RIGHT_THRESHOLD ? PLUS  : ZERO; 
        joyStickPushed_Y = vy < DOWN_THRESHOLD ? PLUS  : vy > UP_THRESHOLD    ? MINUS : ZERO; 

        MoveCircle(&g_sContext, joyStickPushed_X, joyStickPushed_Y);
     }
}


void MoveCircle(Graphics_Context *g_sContext_p, direction_t joyStickPushed_X, direction_t joyStickPushed_Y)
{
    static unsigned int x = 63, y = 63;
    static unsigned int moveCount = 0;
    char string[4];

    if (((x > 20 && joyStickPushed_X < 0) || (x < 107 && joyStickPushed_X > 0)) || ((y > 20 && joyStickPushed_Y < 0) || (y < 107 && joyStickPushed_Y > 0)))
    {

        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
        Graphics_fillCircle(g_sContext_p, x, y, 10);

        if (x > 20 && x < 107) x += joyStickPushed_X * 10;
        if (y > 20 && y < 107) y += joyStickPushed_Y * 10;

        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_YELLOW);
        Graphics_fillCircle(g_sContext_p, x, y, 10);

        moveCount++;
        make_3digit_NumString(moveCount, string);
        Graphics_drawString(g_sContext_p, (int8_t *) string, -1, 10, 110, true);
    }

}

void initialize()
{
    // stop the watchdog timer
    WDT_A_hold(WDT_A_BASE);

    initADC();
    initJoyStick();
    startADC();
}


// Initializing the ADC which resides on SoC
void initADC() {
    ADC14_enableModule();

    ADC14_initModule(ADC_CLOCKSOURCE_SYSOSC,
                     ADC_PREDIVIDER_1,
                     ADC_DIVIDER_1,
                      0
                     );

    // This configures the ADC to store output results
    // in ADC_MEM0 for joystick X.
    // if we want to add joystick Y, then, we have to use more memory locations
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);

    // This configures the ADC in manual conversion mode
    // Software will start each conversion.
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
}


void startADC() {
   // Starts the ADC with the first conversion
   // in repeat-mode, subsequent conversions run automatically
   ADC14_enableConversion();
   ADC14_toggleConversionTrigger();
}


// Interfacing the Joystick with ADC (making the proper connections in software)
void initJoyStick() {

    // This configures ADC_MEM0 to store the result from
    // input channel A15 (Joystick X), in non-differential input mode
    // (non-differential means: only a single input pin)
    // The reference for Vref- and Vref+ are VSS and VCC respectively
    ADC14_configureConversionMemory(ADC_MEM0,
                                  ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                  ADC_INPUT_A15,                 // joystick X
                                  ADC_NONDIFFERENTIAL_INPUTS);

    // This selects the GPIO as analog input
    // A15 is multiplexed on GPIO port P6 pin PIN0
    // which one of GPIO_PRIMARY_MODULE_FUNCTION, or
    //                    GPIO_SECONDARY_MODULE_FUNCTION, or
    //                    GPIO_TERTIARY_MODULE_FUNCTION
    // should be used in place of 0 as the last argument?
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
                                               GPIO_PIN0,
                                               GPIO_TERTIARY_MODULE_FUNCTION);

    // add joystick Y
    ADC14_configureConversionMemory(ADC_MEM1,
                                    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A9, // joystick Y
                                    ADC_NONDIFFERENTIAL_INPUTS);

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
                                               GPIO_PIN4,
                                               GPIO_TERTIARY_MODULE_FUNCTION);

}

void getSampleJoyStick(unsigned *X, unsigned *Y) {
    // ADC runs in continuous mode, we just read the conversion buffers
    *X = ADC14_getResult(ADC_MEM0);

    // Read the Y channel
    *Y = ADC14_getResult(ADC_MEM1);

}


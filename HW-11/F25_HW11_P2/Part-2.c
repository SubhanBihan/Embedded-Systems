#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "ButtonLED_HAL.h"

// Put the correct load value for the PWM period in terms of counter
// cycles based on request PWM frequency,
//       processor clock source of 3MHz and clock divider chosen in this project
#define PWM_PERIOD_CYCLES 1250

#define BLUE_LED_PORT GPIO_PORT_P5
#define BLUE_LED_PIN GPIO_PIN6
#define RED_LED_PORT GPIO_PORT_P2
#define RED_LED_PIN GPIO_PIN6

// Duty cycles as a fraction (between 0 and 1)
#define BLUE_DUTY_CYCLE_FRACTION 0.05
#define RED_DUTY_CYCLE_FRACTION 0.5

// The number of on cycles (Timer_A counter cycles)
#define BLUE_ON_CYCLES (BLUE_DUTY_CYCLE_FRACTION * PWM_PERIOD_CYCLES)
#define RED_ON_CYCLES (RED_DUTY_CYCLE_FRACTION * PWM_PERIOD_CYCLES)

// The number of off cycles (Timer_A counter cycles)
#define BLUE_OFF_CYCLES ((1 - BLUE_DUTY_CYCLE_FRACTION) * PWM_PERIOD_CYCLES)
#define RED_OFF_CYCLES ((1 - RED_DUTY_CYCLE_FRACTION) * PWM_PERIOD_CYCLES)

// Since we start at "off" cycle, the first change comes once the number of
// "off" cycles are over With a different starting point or a different timer_A
// mode, this value could be different to achieve the same duty cycle
#define BLU_COMPARE_CYCLES BLUE_OFF_CYCLES
#define RED_COMPARE_CYCLES RED_OFF_CYCLES

// The below set of choices are not programmer's.
// We need to study the Booster board and Launchpad board diagrams for this
// info.

// The BLUE LED is wired to TA2.1.
// As the programmer we cannot change this. It is decided by the board designer
#define BLU_TIMER TIMER_A2_BASE
#define BLU_CHANNEL TIMER_A_CAPTURECOMPARE_REGISTER_1
// The RED LED is wired to TA0.3.
#define RED_TIMER TIMER_A0_BASE
#define RED_CHANNEL TIMER_A_CAPTURECOMPARE_REGISTER_3

void initPWMPins() {
  // For the Blue LED, we first configure it as a GPIO and turn it off
  // Then, we configure it to work as output channel of a timer
  // The blue LED on booster is wried to Port 5, Pin 6, we cannot change this as
  // the programmer BLUE LED
  GPIO_setAsOutputPin(BLUE_LED_PORT, BLUE_LED_PIN);
  GPIO_setOutputLowOnPin(BLUE_LED_PORT, BLUE_LED_PIN);
  GPIO_setAsPeripheralModuleFunctionOutputPin(BLUE_LED_PORT, BLUE_LED_PIN,
                                              GPIO_PRIMARY_MODULE_FUNCTION);
  // Same for Red LED
  GPIO_setAsOutputPin(RED_LED_PORT, RED_LED_PIN);
  GPIO_setOutputLowOnPin(RED_LED_PORT, RED_LED_PIN);
  GPIO_setAsPeripheralModuleFunctionOutputPin(RED_LED_PORT, RED_LED_PIN,
                                              GPIO_PRIMARY_MODULE_FUNCTION);
}

int main(void) {
  // Stop WDT
  WDT_A_holdTimer();

  // Config struct for Blue LED's PWM
  Timer_A_PWMConfig pwmConfig_blu;
  pwmConfig_blu.clockSource =
      TIMER_A_CLOCKSOURCE_SMCLK;  // The source clock is the system clock (3MHz)
  pwmConfig_blu.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_24;
  pwmConfig_blu.compareOutputMode =
      TIMER_A_OUTPUTMODE_SET_RESET;  // The OC mode. We choose this such that we
                                     // can achieve the waveform we are
                                     // interested in.
  pwmConfig_blu.compareRegister =
      BLU_CHANNEL;  // The output register used for OC operation. The programmer
                    // does not choose this.
  pwmConfig_blu.timerPeriod =
      PWM_PERIOD_CYCLES;  // The # of counter cycles in one PWM round (period of
                          // PWM in terms of counter cycle)
  pwmConfig_blu.dutyCycle =
      BLU_COMPARE_CYCLES;  // The OC value. This is the value that when the
                           // counter hits it, something happens to the waveform

  // Config struct for Red LED's PWM
  Timer_A_PWMConfig pwmConfig_red;
  pwmConfig_red.clockSource =
      TIMER_A_CLOCKSOURCE_SMCLK;  // The source clock is the system clock (3MHz)
  pwmConfig_red.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_7;
  pwmConfig_red.compareOutputMode =
      TIMER_A_OUTPUTMODE_SET_RESET;  // The OC mode. We choose this such that we
                                     // can achieve the waveform we are
                                     // interested in.
  pwmConfig_red.compareRegister =
      RED_CHANNEL;  // The output register used for OC operation. The programmer
                    // does not choose this.
  pwmConfig_red.timerPeriod =
      PWM_PERIOD_CYCLES;  // The # of counter cycles in one PWM round (period of
                          // PWM in terms of counter cycle)
  pwmConfig_red.dutyCycle =
      RED_COMPARE_CYCLES;  // The OC value. This is the value that when the
                           // counter hits it, something happens to the waveform

  initPWMPins();

  // Blue LED's PWM starts
  Timer_A_generatePWM(BLU_TIMER, &pwmConfig_blu);
  // Red LED's PWM starts
  Timer_A_generatePWM(RED_TIMER, &pwmConfig_red);

  while (1) {
  }
}

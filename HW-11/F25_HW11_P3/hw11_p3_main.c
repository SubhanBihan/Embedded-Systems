#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "ButtonLED_HAL.h"

// TODO: Put the correct load value for the PWM period in terms of counter
// cycles based on request PWM frequency,
//       processor clock source of 3MHz and clock divider chosen in this project
#define PWM_PERIOD_CYCLES 284

// Duty cycle as a fraction (between 0 and 1)
#define DUTY_CYCLE_FRACTION 0.5

// The number of on cycles (Timer_A counter cycles)
#define BUZZER_ON_CYCLES (DUTY_CYCLE_FRACTION * PWM_PERIOD_CYCLES)

// The number of off cycles (Timer_A counter cycles)
#define BUZZER_OFF_CYCLES ((1 - DUTY_CYCLE_FRACTION) * PWM_PERIOD_CYCLES)

// Since we start at "off" cycle, the first change comes once the number of
// "off" cycles are over With a different starting point or a different timer_A
// mode, this value could be different to achieve the same duty cycle
#define BUZZER_COMPARE_CYCLES BUZZER_OFF_CYCLES

// The below set of choices are not programmer's.
// We need to study the Booster board and Launchpad board diagrams for this
// info.

// TODO: Set the following values to the correct values for the buzzer - Already correct
#define BUZZER_TIMER TIMER_A0_BASE
#define BUZZER_CHANNEL TIMER_A_CAPTURECOMPARE_REGISTER_4

void initPWMPins() {
  // TODO: Set the following values to the correct values for the buzzer - Already correct
  GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN7);
  GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7);
  GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN7,
                                              GPIO_PRIMARY_MODULE_FUNCTION);
}

// The amount of Timer32 cycles that the note should be played for
// Processor source clock is 3MHz, but Timer32 prescaler is set to 1
#define NOTE_DURATION 9000000

int main(void) {
  // Stop WDT
  WDT_A_holdTimer();

  Timer_A_PWMConfig pwmConfig_blu;
  pwmConfig_blu.clockSource =
      TIMER_A_CLOCKSOURCE_SMCLK;  // The source clock is the system clock (3MHz)
  pwmConfig_blu.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_24;
  pwmConfig_blu.compareOutputMode =
      TIMER_A_OUTPUTMODE_SET_RESET;  // The OC mode. We choose this such that we
                                     // can achieve the waveform we are
                                     // interested in.
  pwmConfig_blu.compareRegister =
      BUZZER_CHANNEL;  // The output register used for OC operation. The programmer
                    // does not choose this.
  pwmConfig_blu.timerPeriod =
      PWM_PERIOD_CYCLES;  // The # of counter cycles in one PWM round (period of
                          // PWM in terms of counter cycle)
  pwmConfig_blu.dutyCycle =
      BUZZER_COMPARE_CYCLES;  // The OC value. This is the value that when the
                           // counter hits it, something happens to the waveform

  initPWMPins();
  // Initialize a hardware timer to measure how long the note is played
  Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
                     TIMER32_PERIODIC_MODE);
  Timer32_setCount(TIMER32_0_BASE, NOTE_DURATION);

  // The below line starts generating the pulse
  Timer_A_generatePWM(BUZZER_TIMER, &pwmConfig_blu);

  // Start the note duration timer in one-shot mode
  Timer32_startTimer(TIMER32_0_BASE, true);

  while (1) {
      if (Timer32_getValue(TIMER32_0_BASE) == 0) {
          Timer_A_stopTimer(BUZZER_TIMER);
      }
  }
}

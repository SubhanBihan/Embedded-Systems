/*
 * Buzzer.c
 *
 *  Created on: Nov 12, 2025
 *      Author: Richard Harrison
 */

#include "HAL/Buzzer.h"

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// Set the tick per s for the clock source
#define CLKSOURCE_VAL          SYSTEM_CLOCK
#define CLKSOURCE_DIV_VAL      PWM_CLKSOURCE_DIVIDER

// // TODO: Set up timer for duration
// #define DT_DIVIDER            1
// #define DT_DIVIDER_VAL        1
// #define DT_TICKS_PER_MS       1

note_t Note_Construct(float frequency_hz, uint32_t duration_ms, float duty_cycle)
{
    const static float frequency_base = CLKSOURCE_VAL / CLKSOURCE_DIV_VAL;

    note_t note;

    // Set the basic values of notes
    note.frequency_hz = frequency_hz;
    note.duration_ms = duration_ms;
    note.duty_cycle = duty_cycle;

    // Compute the period and compare counts
    note.period_counts = frequency_base / note.frequency_hz;
    note.compare_counts = note.duty_cycle * note.period_counts;

    return note;
}

void Buzzer_Init()
{
    // TODO: Initialize buttons to correct port & pin in header
    GPIO_setAsOutputPin(BUZZER_PORT, BUZZER_PIN);
    GPIO_setOutputLowOnPin(BUZZER_PORT, BUZZER_PIN);
    GPIO_setAsPeripheralModuleFunctionOutputPin(BUZZER_PORT, BUZZER_PIN,
                                                GPIO_PRIMARY_MODULE_FUNCTION);

    // TODO: Set up TimerPWM config

    // TODO: Enable countdown timer

}

void Buzzer_Play(note_t* note)
{
    // TODO: Fill TimerPWM config from note
    Timer_A_PWMConfig pwmConfig;
    pwmConfig.clockSource =
        PWM_CLKSOURCE;  // The source clock is the system clock (3MHz)
    pwmConfig.clockSourceDivider = PWM_CLKSOURCE_DIVIDER;
    pwmConfig.compareOutputMode =
        PWM_COMPARE_OUTPUT_MODE;  // The OC mode. We choose this such that we
                                        // can achieve the waveform we are
                                        // interested in.
    pwmConfig.compareRegister =
        BUZZER_CHANNEL;  // The output register used for OC operation. The programmer
                        // does not choose this.
    pwmConfig.timerPeriod =
        note->period_counts;  // The # of counter cycles in one PWM round (period of
                            // PWM in terms of counter cycle)
    pwmConfig.dutyCycle =
        note->compare_counts;  // The OC value. This is the value that when the
                            // counter hits it, something happens to the waveform

    // Generate PWM
    Timer_A_generatePWM(BUZZER_TIMER, &pwmConfig);

    // // Using SWTimer to play the note for required duration
    // SWTimer_start(&buzzer_timer);
}

void Buzzer_Stop()
{
    Timer_A_stopTimer(BUZZER_TIMER);
}

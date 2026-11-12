/*
 * Buzzer.h
 *
 *  Created on: Nov 12, 2025
 *      Author: Richard Harrison
 */

#ifndef HAL_BUZZER_H_
#define HAL_BUZZER_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "HAL/pitches.h"
#include "HAL/Timer.h"

// TODO: Set the following values to the correct values for the buzzer
#define BUZZER_PORT                 GPIO_PORT_P2
#define BUZZER_PIN                  GPIO_PIN7

#define BUZZER_TIMER                TIMER_A0_BASE
#define BUZZER_CHANNEL              TIMER_A_CAPTURECOMPARE_REGISTER_4

#define BUZZER_DURATION_MS 100
#define BUZZER_DUTY_CYCLE_FRACTION 0.5

#define CURSOR_NOTE  NOTE_A5
#define GRANDMA_NOTE NOTE_D6

// TODO: Set the following values to the correct values for the pwm
#define PWM_CLKSOURCE               TIMER_A_CLOCKSOURCE_SMCLK
#define PWM_CLKSOURCE_DIVIDER       TIMER_A_CLOCKSOURCE_DIVIDER_24
#define PWM_COMPARE_OUTPUT_MODE     TIMER_A_OUTPUTMODE_RESET_SET
#define PWM_COMPARE_REGISTER        BUZZER_CHANNEL

typedef struct {
    float frequency_hz;
    uint32_t duration_ms;
    float duty_cycle;

    uint16_t period_counts;
    uint16_t compare_counts;
} note_t;

note_t Note_Construct(float frequency_hz, uint32_t duration_ms, float duty_cycle);

void Buzzer_Init();

void Buzzer_Play(note_t* note);

void Buzzer_Stop();

#endif /* HAL_BUZZER_H_ */

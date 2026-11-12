/*
 * LED.h
 *
 *  Created on: Dec 29, 2019
 *      Author: Matthew Zhong
 */

#ifndef HAL_LED_H_
#define HAL_LED_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

void initLEDs();

void TurnOn_LL1();
void TurnOff_LL1();
void Toggle_LL1();

void TurnOn_LLB();
void TurnOff_LLB();
void Toggle_LLB();

void TurnOn_LLR();
void TurnOff_LLR();
void Toggle_LLR();

void TurnOn_LLG();
void TurnOff_LLG();
void Toggle_LLG();

void TurnOn_BLR();
void TurnOff_BLR();
void Toggle_BLR();

void TurnOn_BLG();
void TurnOff_BLG();
void Toggle_BLG();

void TurnOn_BLB();
void TurnOff_BLB();
void Toggle_BLB();

#endif /* HAL_LED_H_ */

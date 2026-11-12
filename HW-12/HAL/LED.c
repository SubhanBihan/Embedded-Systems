/*
 * LED.c
 *
 *  Created on: Dec 29, 2019
 *      Author: Matthew Zhong
 */

#include <HAL/LED.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// LL1
void TurnOn_LL1() { GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0); }
void TurnOff_LL1() { GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0); }
void Toggle_LL1() { GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0); }

// LLR
void TurnOn_LLR() { GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0); }

void TurnOff_LLR() { GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0); }

void Toggle_LLR() { GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN0); }

// LLG
void TurnOn_LLG() { GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1); }

void TurnOff_LLG() { GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1); }

void Toggle_LLG() { GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN1); }

// LLB
void TurnOn_LLB() { GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2); }

void TurnOff_LLB() { GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2); }

void Toggle_LLB() { GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN2); }

// BLR
void TurnOn_BLR() { GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN6); }

void TurnOff_BLR() { GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6); }

void Toggle_BLR() { GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN6); }

// BLG
void TurnOn_BLG() { GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4); }

void TurnOff_BLG() { GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4); }

void Toggle_BLG() { GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN4); }

// BLB
void TurnOn_BLB() { GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN6); }

void TurnOff_BLB() { GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN6); }

void Toggle_BLB() { GPIO_toggleOutputOnPin(GPIO_PORT_P5, GPIO_PIN6); }

// The HAL itself is written using Driverlib, so it is much easier to implement.
void initLEDs() {
  // Set LL1, LLB, LLR, LLG as output
  GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);  // LL1
  GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);  // LLR
  GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);  // LLG
  GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);  // LLB

  // Set BLR, BLG, and BLB to OUTPUT.
  GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN6);  // BLR
  GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4);  // BLG
  GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN6);  // BLB

  TurnOff_LL1();
  TurnOff_LLR();
  TurnOff_LLB();
  TurnOff_LLG();

  TurnOff_BLR();
  TurnOff_BLG();
  TurnOff_BLB();
}

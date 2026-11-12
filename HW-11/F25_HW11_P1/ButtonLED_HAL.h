/*
 * ButtonLED_HAL.h
 *
 *  Created on: Sep 15, 2018
 *      Author: Leyla
 */

#ifndef BUTTONLED_HAL_H_
#define BUTTONLED_HAL_H_

void initialize_LaunchpadLED1();
void initialize_LaunchpadLED2_red();
void initialize_LaunchpadLED2_green();
void initialize_LaunchpadLED2_blue();
void initialize_BoosterpackLED_red();
void initialize_BoosterpackLED_green();
void initialize_BoosterpackLED_blue();

void turnOn_LaunchpadLED1();
void turnOn_LaunchpadLED2_red();
void turnOn_LaunchpadLED2_green();
void turnOn_LaunchpadLED2_blue();
void turnOn_BoosterpackLED_red();
void turnOn_BoosterpackLED_green();
void turnOn_BoosterpackLED_blue();

void turnOff_LaunchpadLED1();
void turnOff_LaunchpadLED2_red();
void turnOff_LaunchpadLED2_green();
void turnOff_LaunchpadLED2_blue();
void turnOff_BoosterpackLED_red();
void turnOff_BoosterpackLED_green();
void turnOff_BoosterpackLED_blue();

void toggle_LaunchpadLED1();
void toggle_LaunchpadLED2_red();
void toggle_LaunchpadLED2_green();
void toggle_LaunchpadLED2_blue();
void toggle_BoosterpackLED_red();
void toggle_BoosterpackLED_green();
void toggle_BoosterpackLED_blue();

void initialize_LaunchpadLeftButton();
void initialize_LaunchpadRightButton();

void initialize_BoosterpackTopButton();
void initialize_BoosterpackBottomButton();

char switchStatus_LaunchpadLeft();
char switchStatus_LaunchpadRight();
char switchStatus_BoosterpackTop();
char switchStatus_BoosterpackBottom();

char LaunchpadLeftButton_pressed();
char LaunchpadRightButton_pressed();
char BoosterpackTopButton_pressed();
char BoosterpackBottomButton_pressed();

#endif /* BUTTONLED_HAL_H_ */

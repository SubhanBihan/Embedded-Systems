/*
 * Application.h
 *
 *  Created on: Dec 29, 2019
 *      Author: Matthew Zhong
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <HAL/HAL.h>
#include <HAL/Timer.h>

typedef enum {Red, Green, GreenToRed, RedToGreen} _appSmallFSMstate;
typedef enum {B_Off, B_Blue, B_Cyan, B_Green, B_Yellow, B_Red, B_Magenta, BB1_Once, BB2_Once} _appBoosterFSMState;
typedef enum {RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE, OFF} _RGBstatus;

struct _Application
{
    // Put your application members and FSM state variables here!
    // =========================================================================
    _appSmallFSMstate smallFSMstate;
    _appBoosterFSMState boosterFSMState;

};

typedef struct _Application Application;

// Called only a single time - inside of main(), where the application is constructed
Application Application_construct();

// Called once per super-loop of the main application.
void Application_loop(Application* app_p, HAL* hal_p);


void turnLaunchpadRGB(HAL* hal_p, _RGBstatus RGBstatus);
void turnBoosterpackRGB(HAL* hal_p, _RGBstatus RGBstatus);
_RGBstatus smallFSM(Application* app_p, HAL* hal_p);
_RGBstatus HW6FSM(Application* app_p, HAL* hal_p);

#endif /* APPLICATION_H_ */

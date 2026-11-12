/*
 * This project is related to the problem 2 of HW3, ECE2564 Fall 2025.
 * It is important that you read the PDF homework description to understand what you need to do.
 * The comments and TODO items in this document are not self-sufficient to tell you how to complete this project.
 */


#include <stdio.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// TODO #1: complete the below bitmasks. Intensity mask is given to you as an example.
// You need to change the rest from 0 to the appropriate value.
// The position in this context means the lowest index in the bitmask that contains a 1
#define INTENSITY_MASK (BIT1 | BIT2)
#define COLOR_MASK (BIT3 | BIT4)
#define FLICKER_MASK BIT0

//-----------------------------------
// TODO #2: complete the below bitmask positions. Intensity bit position is given to you as an example.
// You need to change the rest from 0 to the appropriate value.
// The position of the mask is the index of the lowest bit of the mask.
#define INTENSITY_BIT_POS 1
#define COLOR_BIT_POS 3
#define FLICKER_BIT_POS 0

//-----------------------------------
// TODO #3: Use typedef and enum keywords to define three new types called color_t, intensity_t, and flickering_t.
// These new types enumerate different color, intensity, and flickering possibilities.
// For intensity_t, use the format INT_* where * is OFF, LOW, MED, or HIGH
// For flickering_t, use format FLICK_* where * is OFF or ON
// color_t is done for you as an example.
typedef enum {WHITE, BLUE, RED, GREEN} color_t;
typedef enum {INT_OFF, INT_LOW, INT_MED, INT_HIGH} intensity_t;
typedef enum {FLICK_OFF, FLICK_ON} flickering_t;

//-----------------------------------
// TODO #4: There is no action item here for you except for reading the below line.
// In here we are defining a new type LSR_t just to add readability to our code.
typedef unsigned char LSR_t;

//-----------------------------------
// TODO #5: Create a structure that hold the data needed for each light.
// Most of the work is done for you. All you need is to study this and add one element that is missing.
typedef struct {
    LSR_t lightSetting;           // the setting of the light
    unsigned int x;               // the x coordinate of the light
    unsigned int y;               // the y coordinate of the light
} light_t;

// function declarations

// These three functions get an LSR as input and return its separate settings
color_t getColorSetting(LSR_t inLSR);
intensity_t getIntensitySetting(LSR_t inLSR);
flickering_t getFlickeringSetting(LSR_t inLSR);

// These three functions update a specific setting within a given LSR (input is the pointer to the LSR)
void updateIntensity(LSR_t* LSR_p, intensity_t newIntensity);
void updateColor(LSR_t* LSR_p, color_t newColor);
void updateFlicker(LSR_t* LSR_p, flickering_t newFlicker);

// This function gets three different settings of an LSR and creates an LSR out of it
LSR_t makeLSR(intensity_t newIntensity, color_t newColor, flickering_t newFlickering);

// This function increases the light intensity for the LSR pointed by the LSR_p
void increaseLight(LSR_t *LSR_p);



void main() {

    LSR_t tempLSR = 0x9A;

    /* TODO #6:After you implement getColorSetting, getIntensitySetting and getFlickerSetting (find them below main)
     * debug the code until it says "End of TODO #6" with various values instead of 0x9A for tempLSR and make sure
     * your getColorSetting, getIntensitySetting, and getFlickeringSetting functions are working correctly.
     */
    color_t color = getColorSetting(tempLSR);
    intensity_t intensity = getIntensitySetting(tempLSR);
    flickering_t flickering = getFlickeringSetting(tempLSR);
    // End of TODO #6

    /* TODO #7: After you implement updateColorSetting, updateIntensitySetting, updateFlickerigSetting (find them below main)
     *  debug the below three lines.
     */
    updateIntensity(&tempLSR, INT_LOW);
    updateColor(&tempLSR, RED);
    updateFlicker(&tempLSR, FLICK_OFF);
    // End of TODO #7

    /* TODO #8: After you implement makeLSR (find it below main), debug the below line. (Use breakpoints to skip the top part.
     * Change the input setting to other things and make sure makeLSR works properly. (this will be different rounds of debugging.)
     */
    LSR_t myLSR = makeLSR(INT_LOW,RED,FLICK_ON);
    // End of TODO #8

    /* TODO #9: Develop the code for increaseLight (find it below main)
     * Debug the code.
     */
    increaseLight(&myLSR);
    intensity_t newIntensity = getIntensitySetting(myLSR);
    // End of TODO #9

    // The below lines initializes the light system of a stage with 5 lights.

   light_t stageLights[5];
   stageLights[0].lightSetting = makeLSR(INT_OFF, BLUE, FLICK_ON);
   stageLights[1].lightSetting = makeLSR(INT_LOW, GREEN, FLICK_ON);
   stageLights[2].lightSetting = makeLSR(INT_MED, RED, FLICK_OFF);
   stageLights[3].lightSetting = makeLSR(INT_MED, WHITE, FLICK_OFF);
   stageLights[4].lightSetting = makeLSR(INT_HIGH, WHITE, FLICK_OFF);

   stageLights[0].x = 20;
   stageLights[1].x = 55;
   stageLights[2].x = 5;
   stageLights[3].x = 80;
   stageLights[4].x = 35;

   stageLights[0].y = 20;
   stageLights[1].y = 10;
   stageLights[2].y = 75;
   stageLights[3].y = 80;
   stageLights[4].y = 50;


   /* TODO #10: Write a snippet of code that if the x coordinate of a light is greater than 50, then intensity is increased.
    * If the x coordinate and the y coordinate of a light are more than 50, the color should be blue. Otherwise, the light should flicker.
    * You have to use the functions you developed earlier to achieve this.
    */
   
    for (unsigned int i = 0; i < 5; ++i) {
        if (stageLights[i].x > 50) {
            increaseLight(&stageLights[i].lightSetting);
        }

        
        if (stageLights[i].x > 50 && stageLights[i].y > 50) {
            updateColor(&stageLights[i].lightSetting, BLUE);
        } else {
            updateFlicker(&stageLights[i].lightSetting, FLICK_ON);
        }
    }
}

/*
 * input:
 *          LSR_t
 * output:
 *          color_t
 * effect:
 *          This function isolates the color setting of LSR and returns it
 * Example: If the input is 0xFF, the output is RED
 */

color_t getColorSetting(LSR_t inLSR)
{
    // The below line creates a warning because the right side of the assignment is not of type color_t.
    // If you like you can typecast the right side to color_t
    // The typecasted version is like this:
    // color_t colorSetting = (color_t) ((inLSR & COLOR_MASK) >> COLOR_BIT_POS);
    color_t colorSetting = ((inLSR & COLOR_MASK) >> COLOR_BIT_POS);
    return colorSetting;
}

/*
 * input:
 *          LSR_t
 * output:
 *          intensity_t
 * effect:
 *          This function isolates the intensity setting of LSR and returns it
 *
 * Example: If the input is 0xFF, the output is INT_HIGH
 */
intensity_t getIntensitySetting(LSR_t inLSR)
{
    intensity_t intensitySetting = ((inLSR & INTENSITY_MASK) >> INTENSITY_BIT_POS);
    return intensitySetting;
}

/*
 * input:
 *          LSR_t
 * output:
 *          flickering_t
 * effect:
 *          This function isolates the flickering setting of LSR and returns it
 *
 * Example: If the input is 0xFF, the output is FLICK_ON
 */
flickering_t getFlickeringSetting(LSR_t inLSR)
{
    flickering_t flickerSetting = ((inLSR & FLICKER_MASK) >> FLICKER_BIT_POS);
    return flickerSetting;
}

/*
 * input:
 *          LSR_t * (the pointer to the LSR that needs to be updated) and intensity_t (the new intensity)
 * output:
 *          nothing
 * effect:
 *          This function updates the intensity of the LSR pointed by LSR_p to the new intensity
 */
void updateIntensity(LSR_t* LSR_p, intensity_t newIntensity)
{
    *LSR_p &= ~INTENSITY_MASK;
    *LSR_p |= (newIntensity << INTENSITY_BIT_POS);
}

/*
 * input:
 *          LSR_t * (the pointer to the LSR that needs to be updated) and color_t (the new color)
 * output:
 *          nothing
 * effect:
 *          This function updates the color of the LSR pointed by LSR_p to the new color
 *
 */
void updateColor(LSR_t* LSR_p, color_t newColor)
{
    *LSR_p &= ~COLOR_MASK;
    *LSR_p |= (newColor << COLOR_BIT_POS);
}

/*
 * input:
 *          LSR_t * (the pointer to the LSR that needs to be updated) and flickering_t (the new flickering)
 * output:
 *          nothing
 * effect:
 *          This function updates the flickering of the LSR pointed by LSR_p to the new flickering
 */
void updateFlicker(LSR_t* LSR_p, flickering_t newFlicker)
{
    *LSR_p &= ~FLICKER_MASK;
    *LSR_p |= (newFlicker << FLICKER_BIT_POS);
}

/*
 * input:
 *          intensity_t, color_t and flickering_t of the to-be-constructed LSR
 * output:
 *          LSR_t
 * effect:
 *          This function receives three integers representing lamp flickering status, color, and intensity and returns the LSR value
 *          that will create that light combination. Use 0s for unused bits in the LSR.
 *          For example, makeLSR with high intensity, red color and flickering returns 0x17
 */
LSR_t makeLSR(intensity_t newIntensity, color_t newColor, flickering_t newFlickering)
{
    LSR_t lsr = 0;
    updateColor(&lsr, newColor);
    updateIntensity(&lsr, newIntensity);
    updateFlicker(&lsr, newFlickering);
    return lsr;
}

/*
 * input:
 *          LSR_t * (the pointer to the LSR that its light intensity should be increased
 * output:
 *          none
 * effect:
 *          The intensity setting of the light that the pointer is pointing to is increased by one level. If it is HIGH, it stays HIGH.
 */
void increaseLight(LSR_t *LSR_p)
{
    intensity_t tempIntensity = getIntensitySetting(*LSR_p);
    if (tempIntensity != INT_HIGH)
    {
        tempIntensity++;
        updateIntensity(LSR_p, tempIntensity);
    }
}

// Fall 2025 - HW5 - Problem 1 - SB
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
// #include <stdbool.h>

// TODO: add states if needed
typedef enum {S0, S1, S2, S3, S4} state_t;

// TODO: add shapes if needed
typedef enum {CIRCLE, LINE, SQUARE} shape_t;

// TODO: add colors if needed
typedef enum {RED, ORANGE, YELLOW, BLUE, GREEN} color_t;

typedef struct {
    int key; // the integer key
    bool S;  // true when S is pushed, false otherwise
} input_t;

typedef struct {
    bool redraw;
    color_t pen_color;
    shape_t shape;
} output_t;


void InitGraphics(Graphics_Context *);
void InitFonts();
void Initialize(Graphics_Context *);

output_t FSM(Graphics_Context* g_sContext_p, input_t in);
void process_output(Graphics_Context* g_sContext_p, output_t out);
input_t refresh_input_manual();
input_t refresh_input_simulate();

int main(void) {

    // A structure that holds a large set of characteristics for graphics including foreground color, background color
    Graphics_Context g_sContext;

    Initialize(&g_sContext);

    output_t out;
    input_t in;

    while (1) {

        // TODO: switch between these two functions for getting the input
        in = refresh_input_simulate(); // You can use flash or debugger for this

        //in = refresh_input_manual(); // Use DEBUGGER for this one!!

        // FSM based on input
        out = FSM(&g_sContext, in);

        // take action based on output
        if (out.redraw)
            process_output(&g_sContext, out);
    }
}

// This function initializes the graphics
void InitGraphics(Graphics_Context *g_sContext_p) {

    Graphics_initContext(g_sContext_p,
                         &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_ORANGE);
    Graphics_setBackgroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(g_sContext_p, &g_sFontCmtt16);

    InitFonts();

    Graphics_clearDisplay(g_sContext_p);
}

// This function initializes the fonts we can use
void InitFonts() {
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
}


// The general initialization function
void Initialize(Graphics_Context* g_sContext_p) {
    WDT_A_hold(WDT_A_BASE);

    InitGraphics(g_sContext_p);
}

// This function shows you how to create a simulated input that does not depend on user input
input_t refresh_input_simulate() {

    // i is static. It means it is 0 in the first call and in subsequent calls it is incremented.
    static int i = 0;

    // You can increase the size of this array and put whatever you wish for key and S and test your FSM
    input_t in_array[12] = {{3, 1}, {6, 1}, {5, 1}, {4, 0}, {10, 1}, {6,0}, {6, 1}, {3, 0}, {10, 0}, {19, 1}, {6, 1}, {4, 0}};

    // Part B.i
    // input_t in_array[8] = {{3, 1}, {8, 0}, {24, 1}, {5, 0}, {12, 1}, {8,0}, {24, 1}, {15, 1}};

    // Part B.ii
    // input_t in_array[8] = {{3, 1}, {15, 1}, {8, 0}, {24, 1}, {5, 0}, {15,0}, {6, 1}, {24, 1}};

    input_t in;

    // Using the modulo % operator allows us to stay within the array size even if i is too big
    in.key = in_array[i%12].key;
    in.S = in_array[i%12].S;

    // since this is a simulated input, it rolls very fast. To slow it down, we add this manual wait using a "useless" loop!
    int wait;
    for (wait=0; wait<1000000; wait++);

    i++;
    return(in);
}

// This function fills the input struct by getting user input
input_t refresh_input_manual() {
    int key, S;
    printf("Enter key (an integer) and S (0 or 1) separated by space:");
    scanf("%d %d", &key, &S);
    printf("\n");

    input_t in;
    in.key = key;
    in.S = S;
    return(in);
}

// This function updates the foreground color of the graphic context based on the "color" argument
void change_pen_color(Graphics_Context* g_sContext_p, color_t color) {
    switch(color) {
    case RED:
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_RED);
        break;
    case ORANGE:
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_ORANGE);
        break;
    case YELLOW:
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_YELLOW);
        break;
    case BLUE:
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
        break;
    case GREEN:
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_GREEN);
        break;
        // TODO: Add more cases if needed.
    }
}

// This function wipes and updates the screen based on the input
void process_output(Graphics_Context* g_sContext_p, output_t out) {

    Graphics_Rectangle R;
    R.xMin = 44; R.xMax = 84; R.yMin = 44; R.yMax = 84;

    // wipe the old shape by drawing over it with the black color as the the background
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);

    // since we don't know what old shape to wipe, we should wipe all possible shapes
    // Another approach could "remember" the old shape and wipe only the old shape.
    // In your projects, you can decide which approach is more appropriate for your case
    // Here, we are redrawing with black to wipe
    Graphics_fillCircle(g_sContext_p, 64, 64, 20);
    Graphics_fillRectangle(g_sContext_p, &R);
    // Graphics_drawLine(g_sContext_p, 44, 64, 84, 64);    
    // Graphics_drawLine(g_sContext_p, 44, 65, 84, 65);
    // Graphics_drawLine(g_sContext_p, 44, 66, 84, 66);
    // Graphics_drawLine(g_sContext_p, 44, 67, 84, 67);

    // Graphics_Rectangle R_line;
    // R.xMin = 44; R.xMax = 84; R.yMin = 64; R.yMax = 67;


    // draw the new shape with the newly selected pen color
    change_pen_color(g_sContext_p, out.pen_color);

    switch(out.shape)
    {
    case CIRCLE:
        Graphics_fillCircle(g_sContext_p, 64, 64, 20);
        break;

    case LINE:
        // Graphics_drawLine(g_sContext_p, 44, 64, 84, 64);        
        // Graphics_drawLine(g_sContext_p, 44, 65, 84, 65);
        // Graphics_drawLine(g_sContext_p, 44, 66, 84, 66);
        // Graphics_drawLine(g_sContext_p, 44, 67, 84, 67);
        R.yMin = 64; R.yMax = 67;
        Graphics_fillRectangle(g_sContext_p, &R);
        break;

    case SQUARE:
        Graphics_fillRectangle(g_sContext_p, &R);
        break;

        // TODO: Add more cases if needed
    }
}

/*
 * This function receives the shape, color and redraw arguments and puts them into an output_t struct and returns it.
 */
output_t build_output(color_t color, shape_t shape, bool redraw) {
    output_t out;
    out.pen_color = color;
    out.shape = shape;
    out.redraw = redraw;
    return out;
}

/*
 * arguments: struct input_t and graphics context pointer
 * output: struct output_t
 * function: This function changes the FSM state based on the input
 * Graphics context pointer is passed so that we can print some information about the FSM on the display and is not necessary.
 */
output_t FSM(Graphics_Context* g_sContext_p, input_t in) {

    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_ORANGE);

    // currentState is declared as a static variable. It means it keeps its value from one call to another.
    // The first time the function is called, the currentState is assigned to be S0.
    // However, the subsequent calls will ignore this line.
    static state_t currentState = S0;
    static unsigned int round_num = 1;
    char round_str[18];

    snprintf(round_str, sizeof(round_str), "Round %d", round_num);

    Graphics_drawString(g_sContext_p, round_str, -1, 10, 10, true);

    output_t out;

    // the default is that the shape/color is not changing and we don't need to re-draw it.
    out.redraw = false;

    switch (currentState) {
    case S0:
        // Edge E0
        // There is no condition on the transition, so we just assign the next state
        // We also set all outputs
        currentState = S1;
        out.pen_color = RED;
        out.shape = CIRCLE;
        out.redraw = true;

        // Extra information printed on display (not needed for FSM functionality)
        // The longest string we write of this nature is "Staying on S1", to cover that we need some empty spaces after our string.
        // Another method could simply wipe the previous string before writing the new one
        Graphics_drawString(g_sContext_p, (int8_t *)"S0 to S1     ", -1, 10, 100, true);

        // Don't forget break!!
        break;

    case S1:
        // Edge E1
        if (in.key <= 6  && !in.S) {
            currentState = S2;
            // to make your code readable, you can create small functions such as the below one (defined earlier in the file)
            out = build_output(YELLOW, LINE, true);

            Graphics_drawString(g_sContext_p, (int8_t *)"S1 to S2     ", -1, 10, 100, true);
        }
        else if (((in.key < 14 && in.key > 7) || in.key > 30) && !in.S) {
            currentState = S4;
            // to make your code readable, you can create small functions such as the below one (defined earlier in the file)
            out = build_output(RED, LINE, true);

            Graphics_drawString(g_sContext_p, (int8_t *)"S1 to S4     ", -1, 10, 100, true);
        }
        else if (in.key >= 15 && in.key < 20 && in.S) {
            currentState = S3;
            // to make your code readable, you can create small functions such as the below one (defined earlier in the file)
            out = build_output(GREEN, SQUARE, true);

            Graphics_drawString(g_sContext_p, (int8_t *)"S1 to S3     ", -1, 10, 100, true);
        }
        else
            Graphics_drawString(g_sContext_p, (int8_t *)"Staying on S1", -1, 10, 100, true);

        // Don't forget break!!
        break;
    
    case S2:
        // Edge E1
        if (in.key == 12  && in.S) {
            currentState = S3;
            // to make your code readable, you can create small functions such as the below one (defined earlier in the file)
            out = build_output(BLUE, CIRCLE, true);

            Graphics_drawString(g_sContext_p, (int8_t *)"S2 to S3     ", -1, 10, 100, true);
        }
        else if (in.key > 14  && !in.S) {
            currentState = S4;
            // to make your code readable, you can create small functions such as the below one (defined earlier in the file)
            out = build_output(BLUE, SQUARE, true);

            Graphics_drawString(g_sContext_p, (int8_t *)"S2 to S4     ", -1, 10, 100, true);
        }
        else
            Graphics_drawString(g_sContext_p, (int8_t *)"Staying on S2", -1, 10, 100, true);

        // Don't forget break!!
        break;
    
    case S3:
        // Edge E1
        if (in.key >= 8  && !in.S) {
            currentState = S4;
            // to make your code readable, you can create small functions such as the below one (defined earlier in the file)
            out = build_output(GREEN, CIRCLE, true);

            Graphics_drawString(g_sContext_p, (int8_t *)"S3 to S4     ", -1, 10, 100, true);
        }
        else
            Graphics_drawString(g_sContext_p, (int8_t *)"Staying on S3", -1, 10, 100, true);

        // Don't forget break!!
        break;
    
    case S4:
        // Edge E1
        if (in.key == 24  && in.S) {
            currentState = S1;
            // to make your code readable, you can create small functions such as the below one (defined earlier in the file)
            out = build_output(ORANGE, SQUARE, true);

            Graphics_drawString(g_sContext_p, (int8_t *)"S4 to S1     ", -1, 10, 100, true);
        }
        else
            Graphics_drawString(g_sContext_p, (int8_t *)"Staying on S4", -1, 10, 100, true);

        // Don't forget break!!
        break;

    }

    round_num++;

    return out;
}



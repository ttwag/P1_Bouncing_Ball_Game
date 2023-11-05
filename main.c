#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include <stdio.h>
#include"LcdDriver/Crystalfontz128x128_ST7735.h"
#include "sys/_stdint.h"
#include "ti/devices/msp432p4xx/driverlib/gpio.h"
#include "ti/devices/msp432p4xx/driverlib/systick.h"
//*****************************************************************************
Graphics_Context g_sContext;

// Global flag
volatile uint32_t flag;
int32_t vx = -1, vy = -1;

static volatile int16_t xADCResult,yADCResult;

void SysTick_Handler(void){
   flag = 1;
}
//*****************************************************************************
void LCD_init(){
   Crystalfontz128x128_Init(); //LCD Screen Size 128x128
   Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
   Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128,&g_sCrystalfontz128x128_funcs);
   Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
   Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
   GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
   Graphics_clearDisplay( &g_sContext);
}

//*****************************************************************************
void erase(int32_t x, int32_t y, int32_t radius){
   Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);//paint dot white to "erase"
   Graphics_fillCircle(&g_sContext, x, y, radius);//set circle radius
}

//*****************************************************************************
void draw(int32_t x, int32_t y, int32_t radius){
   Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);//set dot color again
   Graphics_fillCircle(&g_sContext, x, y, radius); //paint red dot
}

void draw_rec(uint16_t x_left, uint16_t x_right, uint16_t y_down, uint16_t y_top)
{
    assert(x_left < x_right);
    assert(y_top < y_down);

    Graphics_Rectangle rec =
    {
      .xMax = x_right-1,
      .xMin = x_left+1,
      .yMax = y_down-1,
      .yMin = y_top+1
    };
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLUE);
    Graphics_fillRectangle(&g_sContext,&rec);
}

void erase_rec(uint16_t x_left, uint16_t x_right, uint16_t y_down, uint16_t y_top)
{
    assert(x_left < x_right);
    assert(y_top < y_down);

    Graphics_Rectangle rec =
    {
      .xMax = x_right-1,
      .xMin = x_left+1,
      .yMax = y_down-1,
      .yMin = y_top+1
    };
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);//paint dot white to "erase"
    Graphics_fillRectangle(&g_sContext,&rec);
}
void ADC_init()
{
    xADCResult = 0;
    yADCResult = 0;

    /* Configures Pin 6.0 and 4.4 as ADC input */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);

    /* Initializing ADC (ADCOSC/64/8) */
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_64, ADC_DIVIDER_8, 0);

    /* Configuring ADC Memory (ADC_MEM0 - ADC_MEM1 (A15, A9)  with repeat)
         * with internal 2.5v reference */
    MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);
    MAP_ADC14_configureConversionMemory(ADC_MEM0,
            ADC_VREFPOS_AVCC_VREFNEG_VSS,
            ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS);

    MAP_ADC14_configureConversionMemory(ADC_MEM1,
            ADC_VREFPOS_AVCC_VREFNEG_VSS,
            ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);

    /* Enabling the interrupt when a conversion on channel 1 (end of sequence)
     *  is complete and enabling conversions */
    MAP_ADC14_enableInterrupt(ADC_INT1);

    /* Enabling Interrupts */
    MAP_Interrupt_enableInterrupt(INT_ADC14);
    MAP_Interrupt_enableMaster();

    /* Setting up the sample timer to automatically step through the sequence
     * convert.
     */
    MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

    /* Triggering the start of the sample */
    MAP_ADC14_enableConversion();
    MAP_ADC14_toggleConversionTrigger();
}

void ADC14_IRQHandler(void)
{
    uint64_t status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);

    if (ADC_INT1 & status)
    {
        xADCResult = MAP_ADC14_getResult(ADC_MEM0);

        yADCResult = MAP_ADC14_getResult(ADC_MEM1);

        MAP_ADC14_toggleConversionTrigger();

    }
}


//*****************************************************************************
int main(void) {
    // Initialize circle
    int32_t old_x = 10, old_y = 20;
    int32_t x = 10, y = 20; //set start pt and set pos inc
    int32_t count = 0;
    int32_t old_count = 0;
    const int32_t radius = 4; //set radius of circle
    char string[10];
    int32_t period= 100000; //Controls refresh rate and therefore velocity of moving dot, 150000 slowish


    LCD_init();
    ADC_init();
   
    // TODOs: initialize systick,
    WDT_A_hold(WDT_A_bASE);
    SysTick_setPeriod(period);
    SysTick->VAL = 0;
    SysTick_enableModule();
    SysTick_enableInterrupt();


    int16_t x_left = 25, y_top = 50;
    int16_t x_right = 127-x_left, y_down = 127-y_top;
    int32_t x_dir = 0; // flags for the resize of rectangle
    int32_t y_dir = 0;
    draw_rec(x_left,x_right,y_down,y_top);
    draw(x, y, radius);



    // The graphics functions to update the LCD display are constantly looping and delayed
    // until the systick timer reaches one full period.
    while(1) {
        // TODOs for your main loop:
       // 1. wait until you receive the timer flag;
        while (flag == 1) {
            // 2. check the adc (joystick) value and set rectangle resize flags.
            
            // If joystick moves to the right, make rec narrower
            if (xADCResult > 11000) {
                x_dir = 1;
            }
            // If joystick moves to the left, make rec wider
            if (xADCResult < 5000) {
                x_dir = -1;
            }
            // If joystick moves up, make rec taller
            if (yADCResult > 12000) {
                y_dir = 1;
            }
            // if joystick moves down, make rec shorter
            if (yADCResult < 7000) {
                y_dir = -1;
            }
            // 3. some if,else statements and erase_rec, draw_rec functions to resize the rectangle
            
            // If joystick moves to the right
            if (x_dir == 1) {
                erase_rec(x_left,x_right,y_down,y_top);
                x_left = x_left - 10;       // Expand the rectangle horizontally by moving x_left further left
                x_right = 127 - x_left;     // x_right is automatically larger because x_left is smaller
                if (x_left < 0) {
                    x_left = 25;
                    x_right = 127 - x_left;
                }
                draw_rec(x_left,x_right,y_down,y_top);
                erase(x, y, radius);
                //draw(120, 120, radius);
            }

            // If joystick moves to the left
            if (x_dir == -1) {
                erase_rec(x_left,x_right,y_down,y_top);
                x_left = x_left + 10;       // Shrink the rectangle horizontally
                x_right = 127 - x_left;
                if (x_left >= 60) {
                    x_left = 25;
                    x_right = 127 - x_left;
                }
                draw_rec(x_left,x_right,y_down,y_top);
                erase(x, y, radius);
                //draw(120, 5, radius);
            }

            // If joystick moves up
            if (y_dir == 1) {
                erase_rec(x_left,x_right,y_down,y_top);
                y_top = y_top - 10;     // Expand the rectangle vertically by moving the upper boundary further upward
                y_down = 127 - y_top;
                if (y_top < 0) {
                    y_top = 50;
                    y_down = 127 - y_top;
                }
                draw_rec(x_left,x_right,y_down,y_top);
                erase(x, y, radius);
                //draw(5, 120, radius);
            }

            // If joystick moves down
            if (y_dir == -1) {
                erase_rec(x_left,x_right,y_down,y_top);
                y_top = y_top + 10;     // Shrink the rectangle vertically
                y_down = 127 - y_top;
                if (y_top > 60) {
                    y_top = 50;
                    y_down = 127 - y_top;
                }
                draw_rec(x_left,x_right,y_down,y_top);
                erase(x, y, radius);
                //draw(5, 5, radius);
            }
          



            // 4. clear flags, both timer and rectangle resize flags
            x_dir = 0;
            y_dir = 0;
            // 5. clear systick VAL
            
            
            // 6. move the ball
            flag = 0;


            // Prevent the ball from hitting the walls
            // If the ball hits the right boundary
            if (x >= 123) {
                x = 123;
                vx = -vx;      // Flips the direction
                count++;       // Records the wall hitting instance
            }
            // If the ball hits the left boundary
            if (x <= 4) {
                x = 4;
                vx = -vx;
                count++;
            }
            // If the ball hits the lower boundary
            if (y >= 123) {
                y = 123;
                vy = -vy;
                count++;
            }
            // If the ball hits the upper boundary
            if (y <= 4) {
                y = 4;
                vy = -vy;
                count++;
            }
            // Prevent the ball from hitting the rectangle

            // If the ball hits the left side of the rectangle
            if (x + radius == x_left) {
                // If the y-position of the ball is in-between y_top and y_botton
                if ((y + radius <= y_down) && (y - radius >= y_top)) {
                    x = x_left - 4;
                    vx = -vx;
                    count++;
                }
                // If the y-position of the ball is half outside of y_down and half in
                else if ((y + radius >= y_down) && (y - radius <= y_down)) {
                    x = x_left - 4;
                    vx = -vx;
                    count++;
                }
                // If the y-position of the ball is half outside of y_top and half in
                else if ((y - radius <= y_top) && (y + radius >= y_top)) {
                    x = x_left - 4;
                    vx = -vx;
                    count++;
                }
            }
            // If the ball hits the right side of the rectangle
            if (x - radius == x_right) {
                // If the y-position of the ball is in-between the y_top and y_down.
                if ((y + radius <= y_down) && (y - radius >= y_top)) {
                    x = x_right + 4;
                    vx = -vx;  // Reverse the ball
                    count++;
                }
                // If the y-position of the ball half lies out of y_down and half-in
                else if ((y + radius >= y_down) && (y - radius <= y_down)) {
                    x = x_right + 4;
                    vx = -vx;
                    count++;
                }
                // If the y-position of the ball half lies out of y_top and half-in
                else if ((y - radius <= y_top) && (y + radius >= y_top)) {
                    x = x_right + 4;
                    vx = -vx;
                    count++;
                }
            }
            // When the ball hits the lowoer side of the rectangle
            if (y - radius == y_down) {
                // If the x-position of the ball is between x_left and x_right
                if ((x - radius >= x_left) && (x + radius <= x_right)) {
                    y = y_down + 4;
                    vy = -vy;
                    count++;
                }
                // If the x-position of the ball is hald outside of x_left and half in
                else if ((x - radius <= x_left) && (x + radius >= x_left)) {
                    y = y_down + 4;
                    vy = -vy;
                    count++;
                }
                // If the x_position of the ball is hald outside of x_right and half in
                else if ((x - radius <= x_right) && (x + radius >= x_right)) {
                    y = y_down + 4;
                    vy = -vy;
                    count++;
                }
            }
            // if the ball hits the upper side of the rectangle
            if (y + radius == y_top) {
                // If the x-position of the ball is in-between x_left and x_right
                if ((x - radius >= x_left) && (x + radius <= x_right)) {
                    y = y_top - 4;
                    vy = -vy;
                    count++;
                }
                // If the x-position of the ball is half out of x_left and half_in
                else if ((x - radius <= x_left) && (x + radius >= x_left)) {
                    y = y_top - 4;
                    vy = -vy;
                    count++;
                }
                // If the x_position of the ball is half out of x_right and half in
                else if ((x - radius <= x_right) && (x + radius >= x_right)) {
                    y = y_top - 4;
                    vy = -vy;
                    count++;
                }
            }
            // If the present location of the ball is inside of the rectangle
            if ((old_x + radius < x_right) && (old_x - radius > x_left) && (old_y + radius < y_down) && (old_y - radius > y_top)) {
                x = 4;                                          // Redraw the circle instantly
                y = 4;
                old_x = 4;          
                old_y = 4;
                erase(old_x, old_y, radius);                    // Erase the present circle
                draw_rec(x_left, x_right, y_down, y_top);       // Draw a new rectangle to fill the hole
            }             
            // Else keep updating the ball's position as usual
            else {
                erase(old_x, old_y, radius);
                draw(x, y, radius);                 
                old_x = x;
                old_y = y;
                x = x + vx;
                y = y + vy;
            }
            if (count != old_count) {                   // Only re-print count if it changes (hits the wall)
                sprintf(string, "Count: %d", count);
                Graphics_drawStringCentered(&g_sContext,
                                            (int8_t *)string,
                                            AUTO_STRING_LENGTH,
                                            64,  // x - position to center around
                                            64,  // y - position to center around
                                            OPAQUE_TEXT);
            }
            old_count = count; // Update old_count every time.
        }   
    }
    return 0;
}
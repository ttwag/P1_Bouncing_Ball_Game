# Bouncing Ball Game
This project uses the Texas Intrument's **MSP432P4XX** board with a booster pack.

The repository includes the main.c but **doesn't** have the supporting library files.

## Summary
The program displays a rectangle at the center of the microcontroller's LCD screen and a ball. The ball is moving constantly and bounces off the edge of the screen and the rectangle.
We could resize the rectangle with a joystick in the microcontroller's booster pack. It has an analog to digital converter that detects movement of joystick and communicate it to the microcontroller. In addition, the LCD screen updates a count every time the ball hits the rectangle or the edge of the screen.

## Helpful Links
You could learn more about the library function [here](https://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP432_Driver_Library/3_21_00_05/exports/driverlib/msp432_driverlib_3_21_00_05/doc/MSP432P4xx/html/driverlib_html/modules.html)

## Description of the Program
The overall flow of the program goes as follows. 
* First, initialize all the necessary variables and draw a rectangle with the circle outside. 
* Then enable SysTick interrupt and enter an infinite while loop.
* When the SysTick timer flag is raised, the program detects if the joystick is moved.
* If it’s moved up, then the rectangle will expand vertically.
* If it moves down, the rectangle will shrink vertically.
* If it’s moved to the right, the program will expand horizontally.
* If it’s moved to the left, the program will shrink horizontally.
* After checking the status of the joystick, the program checks if the ball hits the wall of the LCD screen or the rectangle.
* If yes, it updates the coordinate of the circle for it to be at the wall, reverses the corresponding velocity, erases the old circle, and prints a new circle according to the updated coordinates.
* Every time when the ball hits a wall, a variable count is updated. At the end of the while loop body, the program will display count if it is changed in-between the last iteration. 

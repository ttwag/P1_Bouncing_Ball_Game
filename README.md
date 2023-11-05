# Bouncing Ball Game
![Figure1](./images/Figure1.png)

## Skills Involved
* C Language
* Embedded System Programming

## Development Environment
I did this project while I attended the class, [EEC7](https://ece.ucdavis.edu/course-catalog), at UC Davis.

This project uses the Texas Instrument's **MSP432P4XX** board with a booster pack. I programmed them in TI's [Code Composer Studio](https://www.ti.com/tool/CCSTUDIO)

## Summary
The program displays a rectangle at the center of the microcontroller's LCD screen and a ball. The ball is moving constantly and bounces off the edge of the screen and the rectangle.
We could resize the rectangle with a joystick in the microcontroller's booster pack. It has an analog-to-digital converter that detects the movement of the joystick and communicates it to the microcontroller. In addition, the LCD screen updates a count every time the ball hits the rectangle or the edge of the screen.

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
* Whenever the ball hits a wall, a variable count is updated. At the end of the while loop body, the program will display the count if it is changed in between the last iteration. 

## What Could I Do Better?
I think the most difficult part of the program in this lab is to make sure that the ball bounces off the rectangle when it’s at the intersection of the length and width of the rectangle (corner). Initially, my program could only bounce the ball off when it hit the boundary, and it always went inside of the rectangle when it hit the corner. Later I realized that this error was due to the lack of condition in my detection program.

In addition, the trajectory of the ball is very fixed. In the future, I could add more randomness to the ball's movement and more balls to the program.

## Helpful Links
You can learn more about the library function [here](https://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP432_Driver_Library/3_21_00_05/exports/driverlib/msp432_driverlib_3_21_00_05/doc/MSP432P4xx/html/driverlib_html/modules.html)
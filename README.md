# COMP2215-Project :: LaFortunaDanceBoard

Within this file you will find the instructions and information needed to play my La Fortuna Game

# Menu Screen

On this screen the user is given 2 options.
Press the Left Key in order to play the dance game in NORMAL mode.
Press the Right Key in order to play the dance game in INVERSE mode.

# Normal Mode

Within this mode the user will be shown a series of directions. The user will start with 4 seconds to select the correct answer by selecting either the {UP,DOWN,RIGHT,LEFT,CENTRE} buttons.

Each correct answer will mean that the user has 0.1 seconds less to answer correctly. This keeps on speeding up until a mistake is made.

When a mistake is screen a lose screen is displayed along with your score

# Inverse Mode

This mode is very similar to normal mode with one main difference. Apart from the CENTRE button all the commands are inverted.

For example if a "LEFT" command is displayed the user must press the "RIGHT" key and so on. This provides a challenging mode for those users wanting more of a challenge. 

The same time rules apply for this mode.

# Demo Screen

After 10 seconds of no response in either the lose screen or the menu the program will time out. This will cause the demo screen to display.

The demo screen has instructions to press a button to start alongside colourful random rectangles that get drawn periodicly. This provides the user some light entertainment instead of a boring menu screen.

# Included code

 -> Steve Gunn's code (adapted by Klaus-Peter Zauner) for the lcd
 
 -> Giacomo Meanti's code for creating random integers (from his Space Invaders game)
 
 -> Frank Vahid code (adapted by Klaus-Peter Zauner) for the rios scheduling
 
 I also took insipiration from nbojinov LaTowerRun main state controller code that I changed for my own purposes: https://github.com/nbojinov/LaTowerRun


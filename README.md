# Simon
A game where you press buttons in the correct sequence.

# Description
My final project is the implemention of the game Simon. The system will have four LED lights on the breadboard as a way to signal the player which buttons will be will pressed in what order. The game starts off with the welcome screen, which tells the user to press the button to begin the game. Once the button is pressed, the game starts. In order to win, the player must be able to do nine button presses in a row for the winning sequence. There will be a total of nine rounds, the first round having only one color in sequence up to the ninth round which will have nine LEDs in sequence. If the right sequence is pressed, we will display the score that the player has. The score starts at 0, and every time the user hits the correct sequence, then the score increases by 1. When the score hits 9, the game ends since the user has correctly hit the nine button sequence correctly.

Every LED also has a sound related to it. When the LED lights up, then the sound plays at the same time. When the user presses a button corresponding to the LED, then the LED turns on and the sound plays.

If the wrong sequence is pressed, the game ends and the LCD displays game over. After a few seconds, the welcome screen shows up again, prompting the player to start the game again.

# User guide (Rules, controls, and any special considerations).
The goal of the game is to reach the ninth round and hit the nine LED sequence correctly in order to end the game.

Controls:

A4 - Starts the game

A3 - Red LED

A2 - Green LED

A1 - Yellow LED

A0 - Blue LED



# Technologies and components used in custom lab (AVR Studio 6, ATmega1284, etc.).
AVR Studio 6 and ATmega1284 were used. 4 different colored LEDs and a speaker are used to help the user play the game properly.

# Video Demonstration
https://www.youtube.com/watch?v=JA3gT3UlVmY

# io.h and io.c
These files are used to program the LCD screen.

# Arduino-Jumping-Game
In this project, an Arduino game has been developed using a 16x2 lcd screen.
I created a heart figure as a game hero. 
This heart figure tries to progress in the game by jumping over the monster figures I have created. 
There are small boxes above the monster figures. 
As the player progresses in the game, the points he / she receives are written in the upper right part. 
If the player hits the monsters, the buzzer starts to squeal, and the red led turns on. monsters arrive in front of the player in a random order. 
At the top of the code, all the parts to be used in the program are defined. lcd connections: lcd (11, 9, 6, 5, 4, 3);
The buzzer is connected to pin 7. The led is connected to pin 7. Character definitions are done here: byte graphics [].

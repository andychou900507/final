# final

How to set up this program?
1. Import bbcar.h and PwmIn.h to control the car, and and uLCD_4DGL to utilize the uLCD.
2. Connect the PwmOut and PwmIn of the two servos, four QTI sensor pin, and Tx and Rx of uLCD.
3. Connect the board and then compile and run the program.
4. Press the reset button.
5. Connect the mbed board and the two servos to the power supply.
6. Put the car and the obstacles on the designed map.

What are the results?
1. After setting up the car and pressing the reset button, the car will start to go ahead.
2. The car will utilize the feedback signal transmitted by QTI sensors to determine whether it is going to turn left or right.
3. When the car arrives the special markings, it will react to the special markings accordingly.
 (1) When the pattern 0111 is sensed, the car will turn left at the intersection of the map.
 (2) When the pattern 1110 is sensed, the car will turn right at the intersection of the map.
4. When the pattern 1111 is sensed, the car will turn stop and detect whether there is any obstacle in front of the car. If there is an obstacle 
   at the front, it will turn to the  other path and then go ahead to avoid the obstacle. On the other hand, if there is no obstacles at the front 
   path, it will go ahead.
5. There are three special design in this project:
 (1) The road condition will be shown on the uLCD, if the front car (or object) is too near, it will stop to avoid crash and show "Watch out !" on 
     uLCD. Otherwise, if there is the distance between it and the front object is large enough, the uLCD will display "Go ahead !".
 (2) When the pattern 1111 is senced, the distance of the obstacle will be shown on the uLCD.
 (3) The designed map is creative, it seems like an artwork. Besides, there are many sharp turns on the map, and this designed car can handle all 
     the turns on the map. Thus, it can be shown that the car is so "robust".

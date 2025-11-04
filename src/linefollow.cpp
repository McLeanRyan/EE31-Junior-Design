#include "motorcontrol.h"
#include "linefollow.h"
#include "colordetect.h"
#include <ArduinoHttpClient.h>

LineFollow::LineFollow() {

}


/* followLeft
   Inputs: Motor 
   This function follows a line on the left edge, meaning the colored line is
   to the right of the robot.
*/
void LineFollow::followLeft(Motor& motor, int lineColor) {
    int targetColor = 1; // Blue
    int currentStateColor = 0;
    int currentColor = 0; // Equal to result of color sensor reading w/ some time average (over 3 reads?)
    int colorFeedback = 0;    
    int baseSpeed = 150; 
    int kLine = 0.1; // Feedback coefficient
    // Line Following Control
    while(true) { // In future, replace with distance sensor instead of true
        currentColor = detectColorClass(50);
        if (currentColor == currentStateColor) {
        // Wall Collision
        // int wallDist = disanceSensor.read(); // TO DO, add Distance Sensor
        // int collisionThreshold = 100;
        // if (wallDist < collisionThreshold) {
        //     motor.stop();
        //     return;
        // }
            if (currentColor = 0) {
                motor.tankDrive(baseSpeed+(kLine*colorFeedback), baseSpeed);
            } else {
                motor.tankDrive(baseSpeed, baseSpeed+(kLine*colorFeedback));
            }
            colorFeedback++; 
            delay(50);
        } else if (currentColor == targetColor){
            motor.stop();

            //Swap Target and Current
            int tempColor = targetColor;
            targetColor = currentStateColor;
            currentStateColor = tempColor;
            colorFeedback = 0;
        } else {
            motor.stop();
            return;
        }
    }
}

void LineFollow::followRight(Motor& motor, int lineColor) {
    // Update once followLeft has been tested and works
}


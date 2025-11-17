#include "motorcontrol.h"
#include "linefollow.h"
#include "colorDetect.h"
#include <ArduinoHttpClient.h>

/* followLeft
   Inputs: Motor 
   This function follows a line on the left edge, meaning the colored line is
   to the right of the robot.
*/
void LineFollow::followLeft(Motor& motor, int lineColor) {
    int currentStateColor = 0;
    int currentColor = 0; // Equal to result of color sensor reading w/ some time average (over 3 reads?)
    int colorFeedback = 0;    
    int baseSpeed = 100; 
    int kLine = 0.1; // Feedback coefficient
    // Line Following Control
    while (true) { // In future, replace with distance sensor instead of true
        currentColor = detectColorClass(50);
        if (currentColor == currentStateColor) {
            if (currentColor == 0) {
                motor.tankDrive(baseSpeed + (kLine * colorFeedback), 0);
            } else {
                motor.tankDrive(0, baseSpeed + (kLine * colorFeedback));
            }
            colorFeedback++; 
        } else if (currentColor == lineColor){

            //Swap Target and Current
            int tempColor = lineColor;
            lineColor = currentStateColor;
            currentStateColor = tempColor;
            colorFeedback = 0;
        } else {
            // motor.stop();
            return;
        }
    }
}

void LineFollow::followRight(Motor& motor, int lineColor) {
    int currentStateColor = 0;
    int currentColor = 0;
    int colorFeedback = 0;
    int baseSpeed = 100;
    int kLine = 0.1; // Feedback coefficient

    while (true) {
        currentColor = detectColorClass(50);
        if (currentColor == currentStateColor) {
            if (currentColor == 0) {
                // Mirror of followLeft: drive right wheel when on same side
                motor.tankDrive(0, baseSpeed + (kLine * colorFeedback));
            } else {
                motor.tankDrive(baseSpeed + (kLine * colorFeedback), 0);
            }
            colorFeedback++;
        } else if (currentColor == lineColor) {
            int tempColor = lineColor;
            lineColor = currentStateColor;
            currentStateColor = tempColor;
            colorFeedback = 0;
        } else {
            return;
        }
    }
}


#include "motorcontrol.h"
#include "linefollow.h"
#include "colordetect.h"
#include <ArduinoHttpClient.h>

/* followLeft
   Inputs: Motor 
   This function follows a line on the left edge, meaning the colored line is
   to the right of the robot.
*/
void LineFollow::followRight(Motor& motor, int lineColor) {
    int currentStateColor = 0;
    int currentColor = 0; // Equal to result of color sensor reading w/ some time average (over 3 reads?)
    int colorFeedback = 0;    
    int baseSpeed = 100; 
    int kLine = 1; // Feedback coefficient
    // Line Following Control
    while (true) { // In future, replace with distance sensor instead of true
        currentColor = detectColorClass(25);
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

void LineFollow::followLeft(Motor& motor, int lineColor) {
    int currentStateColor = 0;
    int currentColor = 0; // Equal to result of color sensor reading w/ some time average (over 3 reads?)
    int colorFeedback = 0;    
    int baseSpeed = 100; 
    int kLine = 1; // Feedback coefficient
    // Line Following Control
    while (true) { // In future, replace with distance sensor instead of true
        currentColor = detectColorClass(25);
        if (currentColor == currentStateColor) {
            if (currentColor == 0) {
                motor.tankDrive(0, baseSpeed + (kLine * colorFeedback));
            } else {
                motor.tankDrive(baseSpeed + (kLine * colorFeedback), 0);
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


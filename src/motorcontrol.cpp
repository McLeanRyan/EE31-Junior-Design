/*
 *  motorcontrol.cpp
 *  Hoang Mai
 *  10/02/2025
 *
 *
 *  Motor Implementations 
 */
#include "motorcontrol.h"
#include <ArduinoHttpClient.h>
#include "colorDetect.h"
#include "irDetect.h"
#include "imu.h"

#define LEFT_ENABLE 9
#define LEFT_CC 7
#define LEFT_CW 8

#define RIGHT_ENABLE 3
#define RIGHT_CC 4
#define RIGHT_CW 5

/**********************   HELPER FUNCTION DELETE   ***************************/
void printColor(WebSocketClient &client, int currentColor) {
    /* Print Current Color */
    client.beginMessage(TYPE_TEXT);
    if (currentColor == 0){
        Serial.println("Current Color Reading is Black " );
        client.println("Current Color Reading is Black " );
    } else if (currentColor == 1) {
        Serial.println("Current Color Reading is Blue " );
        client.println("Current Color Reading is Blue " );
    } else if (currentColor == 2) {
        Serial.println("Current Color Reading is Yellow " );
        client.println("Current Color Reading is Yellow " );
    } else if (currentColor == 3) {
        Serial.println("Current Color Reading is Red " );
        client.println("Current Color Reading is Red " );
    }
    client.endMessage();
    delay(500);
}

/*  Motor
    Description: Motor Constructor that enables the coresponding pins
*/
Motor::Motor() 
{
    pinMode(LEFT_ENABLE, OUTPUT);
    pinMode(LEFT_CC, OUTPUT);
    pinMode(LEFT_CW, OUTPUT);

    pinMode(RIGHT_ENABLE, OUTPUT);
    pinMode(RIGHT_CC, OUTPUT);
    pinMode(RIGHT_CW, OUTPUT);
}

/*  driveForward
    Description:    Give an input speed 0-255, drive both motors forward
    Input:          Given speed 0 - 255
    Output:         None
*/
void Motor::driveForward(int speed)
{   
    digitalWrite(LEFT_CW, HIGH);
    digitalWrite(LEFT_CC, LOW);
    digitalWrite(RIGHT_CW, HIGH);
    digitalWrite(RIGHT_CC, LOW);

    analogWrite(LEFT_ENABLE, speed+30);
    analogWrite(RIGHT_ENABLE, speed);
}

/*  driveBackward
    Description:    Give an input speed 0-255, drive both motors forward
    Input:          Given speed 0 - 255
    Output:         None
*/
void Motor::driveBackward(int speed)
{   
    digitalWrite(LEFT_CW, LOW);
    digitalWrite(LEFT_CC, HIGH);
    digitalWrite(RIGHT_CW, LOW);
    digitalWrite(RIGHT_CC, HIGH);

    analogWrite(LEFT_ENABLE, speed);
    analogWrite(RIGHT_ENABLE, speed);
}

/*  stop
    Description: stop the motor
*/
void Motor::stop() 
{
    analogWrite(LEFT_ENABLE, 0);
    analogWrite(RIGHT_ENABLE, 0);
}

/* pivotCC
   Description: Pivot the Robot counter Clockwise*/
void Motor::pivotCCW()
{
    // Speed to use for pivoting
    int speed = 150;

    // Set directions: left forward, right backward
    digitalWrite(LEFT_CW, HIGH);
    digitalWrite(LEFT_CC, LOW);
    digitalWrite(RIGHT_CW, LOW);
    digitalWrite(RIGHT_CC, HIGH);

    // Enable motors
    analogWrite(LEFT_ENABLE, speed);
    analogWrite(RIGHT_ENABLE, speed);

}

/* pivotCCW
   Description: Pivot the Robot clockwise*/
void Motor::pivotCW()
{
    // Speed for in-place pivoting
    int speed = 150;

    // Left motor moves backward
    digitalWrite(LEFT_CW, LOW);
    digitalWrite(LEFT_CC, HIGH);

    // Right motor moves forward
    digitalWrite(RIGHT_CW, HIGH);
    digitalWrite(RIGHT_CC, LOW);

    // Apply same speed to both motors for in-place rotation
    analogWrite(LEFT_ENABLE, speed);
    analogWrite(RIGHT_ENABLE, speed);
}

/* turnRight
   Description: Turns the bot to the right with a given turnRadius factor
*/
void Motor::turnLeft(int turnRadius) 
{
    // turnRadius can control duration — tweak this experimentally
    int outerSpeed = 150;   // right wheel goes slower (inner wheel)
    int innerSpeed = 100;

    // Left wheel forward (outer), Right wheel forward but slower (inner)
    digitalWrite(LEFT_CW, HIGH);
    digitalWrite(LEFT_CC, LOW);
    digitalWrite(RIGHT_CW, HIGH);
    digitalWrite(RIGHT_CC, LOW);

    analogWrite(LEFT_ENABLE, outerSpeed);
    analogWrite(RIGHT_ENABLE, innerSpeed);

    int duration = turnRadius * 10;   // e.g., turnRadius=10 gives ~100ms
    delay(duration);

    stop();
}

/* turnLeft
   Description: Turns the bot to the left with a given turnRadius factor
*/
void Motor::turnRight(int turnRadius) 
{
    int outerSpeed = 150;
    int innerSpeed = 100;

    // Right wheel forward (outer), Left wheel forward but slower (inner)
    digitalWrite(LEFT_CW, HIGH);
    digitalWrite(LEFT_CC, LOW);
    digitalWrite(RIGHT_CW, HIGH);
    digitalWrite(RIGHT_CC, LOW);

    analogWrite(LEFT_ENABLE, innerSpeed);
    analogWrite(RIGHT_ENABLE, outerSpeed);

    int duration = turnRadius * 20;   // can tweak duration value
    delay(duration);

    stop();
}

/* tankDrive
   Description: Drive each wheel forward with individual speed
*/
void Motor::tankDrive(int speedLeft, int speedRight) 
{
    // Both wheels moving forward
    digitalWrite(LEFT_CW, HIGH);
    digitalWrite(LEFT_CC, LOW);
    digitalWrite(RIGHT_CW, HIGH);
    digitalWrite(RIGHT_CC, LOW);

    analogWrite(LEFT_ENABLE, speedLeft);
    analogWrite(RIGHT_ENABLE, speedRight);
}

/* followLane
   Inputs: Left/Right Edge, Motor, client for debugging

   This function follows the line depending on which edge is inputted
*/
void Motor::followLane(Edge side, int lineColor, WebSocketClient &client)
{
    int currentStateColor = COLOR_BLACK;
    int currentColor = 0;
    int colorFeedback = 0;

    int baseSpeed = 120;
    int kLine = 1;

    while (!detectDistance(-280)) {
        currentColor = detectColorClass(5);
        //printColor(client, currentColor);

        if (currentColor == currentStateColor) {

            int boost = baseSpeed + (kLine * colorFeedback);
            int leftPower = 0;
            int rightPower = 0;

            // GENERALIZED COLOR
            if (side == RIGHT_EDGE) {
                // same behavior as your working followRight()
                if (currentColor == 0) {
                    leftPower = boost;   // hugging right, left wheel drives
                    rightPower = 60;
                } else {
                    rightPower = boost;  // saw line, correct by driving right wheel
                    leftPower = 60;
                }
            } 
            else { // LEFT_EDGE
                // mirrored logic
                if (currentColor == 0) {
                    rightPower = boost;  // hugging left, right wheel drives
                    leftPower = 60;
                } else {
                    leftPower = boost;   // saw line, correct by driving left wheel
                    rightPower = 60;
                }
            }

            // Serial.print("Left Power: ");
            // Serial.println(leftPower);
            // Serial.print("Right Power: ");
            // Serial.println(rightPower);

            tankDrive(leftPower, rightPower);
            colorFeedback++;
        } 
        else if (currentColor == lineColor) {
            int temp = lineColor;
            lineColor = currentStateColor;
            currentStateColor = temp;
            colorFeedback = 0;
        } 
        else {
            return;
        }
    }
}


/* followRight
   Inputs: Motor 
   This function follows a line on the right edge, meaning the colored line is
   to the right of the robot.
*/
void Motor::followRight(int lineColor, WebSocketClient &client)
{
    int currentStateColor = 0, currentColor = 0, colorFeedback = 0, boost = 0;
    int baseSpeed = 100; 
    int kLine = 1; // Feedback coefficient


    // Line Following Control
    while (true) { // In future, replace with distance sensor instead of true
        currentColor = detectColorClass(5);

        printColor(client, currentColor);
        /* Print Current Color */
        if (currentColor == currentStateColor) {
            if (currentColor == 0) {
                tankDrive(baseSpeed + (kLine * colorFeedback), 0);
            } else {
                tankDrive(0, baseSpeed + (kLine * colorFeedback));
            }
            colorFeedback++; 
        } else if (currentColor == lineColor) {
            //Swap Target and Current
            int tempColor = lineColor;
            lineColor = currentStateColor;
            currentStateColor = tempColor;
            colorFeedback = 0;
        } else {
            return;
        }
    }
}

void Motor::gyroTurn(double angle) 
{
    // Serial.println("Entered Gyro Turn");
    double kP = 2;
    double kI = 0;
    double kD = 0;
    double kDrive = 1; // Scale output of PID controller to pass as speed
    int baseSpeed = 90;

    imuZeroHeading();
    double current = imuGetHeading();

    double target = angle;
    double totalError, previousError, changeError, PIDOut = 0;
    double e = 90;
    int lastTime = 0;

    while(abs(e) >= 0.5) {
        imuUpdateHeading();
        double current = imuGetHeading();

        unsigned long now = millis();
        double dt = (now - lastTime) / 1000.0;  // Convert to seconds

        e = target -double(current); // REPLACE 0 WITH CURRENT GYRO READING
        totalError += (e * dt);
        changeError = (e - previousError) / dt;
        PIDOut = abs(kDrive * ((kP * e) + (kI * totalError) + (kD * changeError)));
        PIDOut = constrain(PIDOut, baseSpeed, 255);
        // Serial.println(e);
        if (e < 0) {
            digitalWrite(LEFT_CW, LOW);
            digitalWrite(LEFT_CC, HIGH);
            digitalWrite(RIGHT_CW, HIGH);
            digitalWrite(RIGHT_CC, LOW);

            analogWrite(LEFT_ENABLE, PIDOut);
            analogWrite(RIGHT_ENABLE, PIDOut);
        }else if (e > 0){
            digitalWrite(LEFT_CW, HIGH);
            digitalWrite(LEFT_CC, LOW);
            digitalWrite(RIGHT_CW, LOW);
            digitalWrite(RIGHT_CC, HIGH);

            analogWrite(LEFT_ENABLE, PIDOut);
            analogWrite(RIGHT_ENABLE, PIDOut);
        }
        previousError = e;
        lastTime = now;
    }
    analogWrite(LEFT_ENABLE, 0);
    analogWrite(RIGHT_ENABLE, 0);
}

void Motor::gyroDriveToLine(int speed, int color) 
{
    digitalWrite(LEFT_CW, HIGH);
    digitalWrite(LEFT_CC, LOW);
    digitalWrite(RIGHT_CW, HIGH);
    digitalWrite(RIGHT_CC, LOW);

    double kP = 2;
    double kI = 0;
    double kD = 0;

    imuZeroHeading();
    double current = imuGetHeading();

    double target = current; //REPLACE 0 WITH GYRO READING AT START
    double totalError, previousError, changeError, PIDOut = 0;
    int lastTime = 0;

    int currentColor = detectColorClass(5);
    while(!currentColor == color)
    {
        imuUpdateHeading();
        current = imuGetHeading();

        unsigned long now = millis();
        double dt = (now - lastTime) / 1000.0;  // Convert to seconds

        double e = target - double(current); // REPLACE 0 WITH CURRENT GYRO READING
        totalError += (e*dt);
        changeError = (e - previousError)/dt;
        PIDOut = abs((kP * e) + (kI * totalError) + (kD * changeError));
        PIDOut = constrain(PIDOut, 80, 255);

        Serial.println(PIDOut);
        if (e > 0) {
            analogWrite(LEFT_ENABLE, speed + PIDOut);
            analogWrite(RIGHT_ENABLE, speed - PIDOut);
        }else if (e < 0){
            analogWrite(LEFT_ENABLE, speed - PIDOut);
            analogWrite(RIGHT_ENABLE, speed + PIDOut);
        }
        previousError = e;
        lastTime = now;
        currentColor = detectColorClass(5);
    }
    analogWrite(LEFT_ENABLE, 0);
    analogWrite(RIGHT_ENABLE, 0);
}

void Motor::gyroDriveToWall(int speed, int distance) 
{
    Serial.println("Started Gyro Drive");
    digitalWrite(LEFT_CW, HIGH);
    digitalWrite(LEFT_CC, LOW);
    digitalWrite(RIGHT_CW, HIGH);
    digitalWrite(RIGHT_CC, LOW);

    double kP = 0.005;
    double kI = 0.00008;
    double kD = 0.01;

    imuZeroHeading();
    double current = imuGetHeading();

    double target = current; //REPLACE 0 WITH GYRO READING AT START
    double totalError, previousError, changeError, PIDOut = 0;
    int lastTime = 0;

    analogWrite(LEFT_ENABLE, speed);
    analogWrite(RIGHT_ENABLE, speed);

    while(!detectDistance(distance))
    {
        imuUpdateHeading();
        current = imuGetHeading();
        // Serial.println(current);
        unsigned long now = millis();
        double dt = (now - lastTime) / 1000.0;  // Convert to seconds

        double e = target - double(current); // REPLACE 0 WITH CURRENT GYRO READING
        totalError += (e*dt);
        changeError = (e - previousError)/dt;
        PIDOut = abs((kP * e) + (kI * totalError) + (kD * changeError));
        PIDOut = constrain(PIDOut, 80, 255);
        // Serial.println(PIDOut);
        if (e > 0) {
            analogWrite(LEFT_ENABLE, speed + PIDOut);
            analogWrite(RIGHT_ENABLE, speed - PIDOut);
        }else if (e < 0){
            analogWrite(LEFT_ENABLE, speed - PIDOut);
            analogWrite(RIGHT_ENABLE, speed + PIDOut);
        }
        previousError = e;
        lastTime = now;
    }
    analogWrite(LEFT_ENABLE, 0);
    analogWrite(RIGHT_ENABLE, 0);
    return;
}
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

#define LEFT_ENABLE 9
#define LEFT_CC 7
#define LEFT_CW 8

#define RIGHT_ENABLE 3
#define RIGHT_CC 4
#define RIGHT_CW 5

/*  Motor
    Description: Motor Constructor that enables the coresponding pins
*/
Motor::Motor() {
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

    analogWrite(LEFT_ENABLE, speed);
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
void Motor::turnLeft(int turnRadius) {
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
void Motor::turnRight(int turnRadius) {
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
void Motor::tankDrive(int speedLeft, int speedRight) {
    // Both wheels moving forward
    digitalWrite(LEFT_CW, HIGH);
    digitalWrite(LEFT_CC, LOW);
    digitalWrite(RIGHT_CW, HIGH);
    digitalWrite(RIGHT_CC, LOW);

    analogWrite(LEFT_ENABLE, speedLeft);
    analogWrite(RIGHT_ENABLE, speedRight);
}




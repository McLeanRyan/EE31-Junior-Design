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

void handleState(Motor& motor, States state) {
    switch (state) {
        case STOP:
            motor.stop();
            break;

        case FORWARD:
            motor.driveForward(200);      
            break;

        case BACKWARD:
            motor.driveBackward(200);
            break;

        case PivotClockwise:
            motor.pivotCW(90);            
            break;

        case PivotCounterClockwise:
            motor.pivotCCW(90);
            break;

        case TurnRight:
            motor.rightTurn(150);
            break;

        case TurnLeft:
            motor.leftTurn(150);
            break;

        default:
            motor.stop();
            break;
    }
}

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
   Description: Pivot the Robot Clock*/
void Motor::pivotCW(int degree)
{
    delay(1000);
    // Speed to use for pivoting
    int speed = 100;

    // Set directions: left forward, right backward
    digitalWrite(LEFT_CW, HIGH);
    digitalWrite(LEFT_CC, LOW);
    digitalWrite(RIGHT_CW, LOW);
    digitalWrite(RIGHT_CC, HIGH);

    // Enable motors
    analogWrite(LEFT_ENABLE, speed);
    analogWrite(RIGHT_ENABLE, speed);

    delay(1000);

    // --- Timing-based rotation control ---
    // Calibrate this constant experimentally for your robot:
    const float ms_per_degree = 49.0; // adjust this value for accuracy

    int duration = degree * ms_per_degree;

    delay(duration);  // rotate for calculated time

    // Stop both motors after turning
    stop();
}

void Motor::pivotCCW(int degree)
{
    // Speed to use for pivoting
    int speed = 150;

    // Set directions: left backward, right forward
    digitalWrite(LEFT_CW, LOW);
    digitalWrite(LEFT_CC, HIGH);
    digitalWrite(RIGHT_CW, HIGH);
    digitalWrite(RIGHT_CC, LOW);

    // Enable motors
    analogWrite(LEFT_ENABLE, speed);
    analogWrite(RIGHT_ENABLE, speed);

    // Use the same calibration as pivotCW()
    const float ms_per_degree = 49.0;  

    int duration = degree * ms_per_degree;

    delay(duration);  // rotate for calculated time

    // Stop both motors after turning
    stop();
}

void Motor::rightTurn(int turnRadius)
{

}

void Motor::leftTurn(int turnRadius) {

}




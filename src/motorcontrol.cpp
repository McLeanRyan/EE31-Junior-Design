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
#include "imu.h"
#include "colorDetect.h"
#include "irDetect.h"

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
void Motor::leftTurn(int turnRadius) {
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
void Motor::rightTurn(int turnRadius) {
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
        PIDOut = (kP * e) + (kI * totalError) + (kD * changeError);
        
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

    while(!detectDistance(distance))
    {
        imuUpdateHeading();
        current = imuGetHeading();

        unsigned long now = millis();
        double dt = (now - lastTime) / 1000.0;  // Convert to seconds

        double e = target - double(current); // REPLACE 0 WITH CURRENT GYRO READING
        totalError += (e*dt);
        changeError = (e - previousError)/dt;
        PIDOut = (kP * e) + (kI * totalError) + (kD * changeError);
        
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
    double e = 100;
    int lastTime = 0;

    while(abs(e) >= 3) {
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



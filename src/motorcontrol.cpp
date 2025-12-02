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
// IMU wrapper (reads gyroscope)
#include "imu.h"

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
void Motor::turnRight(int turnRadius) {
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
void Motor::turnLeft(int turnRadius) {
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

void gyroDrive(int speed) 
{
    // Set motor directions to forward for both wheels
    digitalWrite(LEFT_CW, HIGH);   // left motor clockwise high => forward
    digitalWrite(LEFT_CC, LOW);    // left motor counter-clockwise low
    digitalWrite(RIGHT_CW, HIGH);  // right motor clockwise high => forward
    digitalWrite(RIGHT_CC, LOW);   // right motor counter-clockwise low

    // PID gains 
    double kP = 1.0;
    double kI = 0.0;
    double kD = 0.0;

    // Read an initial gyro sample to use as the target (baseline) rate
    float gx = 0.0f, gy = 0.0f, gz = 0.0f;
    double target = 0.0; // desired angular rate (deg/s)
    if (imuRead(gx, gy, gz)) {
        // store starting gz as target so we try to maintain initial rotation rate
        target = (double)gz;
    }

    // PID state variables - initialize to zero
    double totalError = 0.0;
    double previousError = 0.0;
    double changeError = 0.0;
    double PIDOut = 0.0;

    // Control loop: run for a fixed number of iterations
    for (int i = 0; i < 1000; i++) {
        // Read current gyro (gz) value
        if (!imuRead(gx, gy, gz)) {
            // if read failed, skip this iteration
            delay(1);
            continue;
        }

        // Compute error between desired target rate and current measured rate
        double e = target - (double)gz;

        // Integrate error (simple integral term)
        totalError += e;

        // Compute change in error (for derivative term)
        changeError = e - previousError;

        // PID output: proportional + integral + derivative
        PIDOut = (kP * e) + (kI * totalError) + (kD * changeError);

        // Apply PID correction to left/right PWM values
        if (e > 0) {
            // If error positive, rotate correction one way
            analogWrite(LEFT_ENABLE, constrain((int)(speed + PIDOut), 0, 255));
            analogWrite(RIGHT_ENABLE, constrain((int)(speed - PIDOut), 0, 255));
        } else if (e < 0) {
            // If error negative, rotate correction the other way
            analogWrite(LEFT_ENABLE, constrain((int)(speed - PIDOut), 0, 255));
            analogWrite(RIGHT_ENABLE, constrain((int)(speed + PIDOut), 0, 255));
        } else {
            // No error: drive straight at base speed
            analogWrite(LEFT_ENABLE, constrain(speed, 0, 255));
            analogWrite(RIGHT_ENABLE, constrain(speed, 0, 255));
        }

        // Save current error for next loop's derivative calculation
        previousError = e;

        // Small sleep to avoid maxing out loop speed
        delay(5);
    }
}

void gyroTurn(double angle) 
{
    double kP = 1;
    double kI = 0;
    double kD = 0;
    double kDrive = 1; // Scale output of PID controller to pass as speed
    int baseSpeed = 100;
    // Read initial gyro z-rate to form a simple baseline
    float gx = 0.0f, gy = 0.0f, gz = 0.0f;
    if (!imuRead(gx, gy, gz)) {
        return; // can't read imu, abort
    }

    // Use current gz reading as the baseline and form a simple target
    double target = angle + (double)gz; // baseline + requested angle
    double totalError = 0.0;
    double previousError = 0.0;
    double changeError = 0.0;
    double PIDOut = 0.0;
    double e = 100.0;

    while (fabs(e) > 0.1) {
        // update current gyro reading
        if (!imuRead(gx, gy, gz)) {
            delay(1);
            continue;
        }

        // compute error using the current gz value
        e = target - (double)gz;
        totalError += e;
        changeError = e - previousError;
        PIDOut = kDrive * ((kP * e) + (kI * totalError) + (kD * changeError));

        int pwm = constrain((int)fabs(PIDOut), 0, 255);

        if (e > 0) {
            digitalWrite(LEFT_CW, LOW);
            digitalWrite(LEFT_CC, HIGH);
            digitalWrite(RIGHT_CW, HIGH);
            digitalWrite(RIGHT_CC, LOW);

            analogWrite(LEFT_ENABLE, pwm);
            analogWrite(RIGHT_ENABLE, pwm);
        } else if (e < 0) {
            digitalWrite(LEFT_CW, HIGH);
            digitalWrite(LEFT_CC, LOW);
            digitalWrite(RIGHT_CW, LOW);
            digitalWrite(RIGHT_CC, HIGH);

            analogWrite(LEFT_ENABLE, pwm);
            analogWrite(RIGHT_ENABLE, pwm);
        } else {
            analogWrite(LEFT_ENABLE, 0);
            analogWrite(RIGHT_ENABLE, 0);
        }

        previousError = e;
        delay(5);
    }

    // stop motors when done
    analogWrite(LEFT_ENABLE, 0);
    analogWrite(RIGHT_ENABLE, 0);
}
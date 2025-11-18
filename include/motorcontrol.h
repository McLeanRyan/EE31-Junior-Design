/*
 *  motorcontrol.h
 *  Hoang Mai
 *  10/02/2025
 *
 *
 *  Motor Class will represent the functionality that a motor could perform
 */
#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

using namespace std;
#include "state.h"

class Motor {
public:
    Motor();

    void stop();
    void driveForward(int speed);
    void driveBackward(int speed);
    void pivotCW();
    void pivotCCW();
    void rightTurn(int turnRadius);
    void leftTurn(int turnRadius);
    void gyroDrive(int speed);

private:
};

#endif
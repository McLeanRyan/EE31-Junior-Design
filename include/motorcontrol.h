/*
 *  motorcontrol.h
 *  10/02/2025
 *
 *
 *  Motor Class will represent the functionality that a motor could perform
 */
#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

using namespace std;
#include "state.h"
#include <ArduinoHttpClient.h>

enum Edge {
    LEFT_EDGE,
    RIGHT_EDGE,
};

class Motor {
public:
    Motor();

    void stop();
    void driveForward(int speed);
    void driveBackward(int speed);
    void pivotCW();
    void pivotCCW();
    void turnRight(int turnRadius);
    void turnLeft(int turnRadius);
    void tankDrive(int speedLeft, int speedRight);
    void followLane(Edge Side, int lineColor, WebSocketClient &client);
    void followLeft(int lineColor, WebSocketClient &client);
    void followRight(int lineColor, WebSocketClient &client);
    void gyroTurn(double angle);
    void gyroDriveToLine(int speed, int color);
    void gyroDriveToWall(int speed, int distance);
    
private:
};

#endif
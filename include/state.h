/*
 *  state.h
 *
 *
 *  Defining the state Implementations,
 */

#ifndef STATES_H
#define STATES_H
#include "motorcontrol.h"
#include <ArduinoHttpClient.h>

// Defining the Available States
enum States {
    STOP = 0,
    FORWARD,
    BACKWARD,
    PivotClockwise,
    PivotCounterClockwise,
    TurnRight,
    TurnLeft,
    FollowLeft,
    FollowRight
};

class Motor;
class LineFollow;

void nextState();
void handleState(Motor& motor, States state, WebSocketClient &client);

#endif

#ifndef STATES_H
#define STATES_H
#include "motorcontrol.h"

// Define your shared state machine enum here
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
void handleState(Motor& motor, States state);

#endif

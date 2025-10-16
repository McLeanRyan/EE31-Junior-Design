#ifndef STATES_H
#define STATES_H

// Define your shared state machine enum here
enum States {
    STOP = 0,
    FORWARD,
    BACKWARD,
    PivotClockwise,
    PivotCounterClockwise,
    TurnRight,
    TurnLeft
};

#endif

#include "motorcontrol.h"
#include "state.h"

void nextState(States state) 
{
  state = static_cast<States>((static_cast<int>(state) + 1) % 7);
}

void handleState(Motor &motor, States state)
{
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
            motor.pivotCW();            
            break;

        case PivotCounterClockwise:
            motor.pivotCCW();
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
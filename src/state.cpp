#include "motorcontrol.h"
#include "state.h"
#include "irDetect.h"

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
            motor.turnRight(150);
            break;

        case TurnLeft:
            motor.turnLeft(150);
            break;

        case CollisionAvoidance:
            motor.driveForward(200);
            while (!detectDistance(-600)) {}
            motor.stop();
            break;

        default:
            motor.stop();
            break;
    }
}
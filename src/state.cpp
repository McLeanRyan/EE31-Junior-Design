/*
 *  state.h
 *
 *  Defining the state Implementations,
 */

#include "state.h"
#include "colorDetect.h"

void nextState(States state) 
{
  state = static_cast<States>((static_cast<int>(state) + 1) % 7);
}

void handleState(Motor& motor, States state, WebSocketClient &client)
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

        case FollowLeft:
            motor.followLane(LEFT_EDGE, COLOR_BLUE, client);
            break;
            
        case FollowRight:
            motor.followLane(RIGHT_EDGE, COLOR_BLUE, client);
            break;

        default:
            motor.stop();
            break;
    }
}
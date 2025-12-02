/*
 *  state.h
 *
 *  Defining the state Implementations,
 */

#include "state.h"
#include "colorDetect.h"
#include "irDetect.h"

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
            if (detectDistance(-500)) {
                motor.stop();
                break;
            }
            motor.driveForward(200);      
            break;

        case BACKWARD:
            motor.driveBackward(200);
            break;

        case PivotClockwise:
            if (detectDistance(-500)) {
                motor.stop();
                break;
            }
            motor.pivotCW();            
            break;

        case PivotCounterClockwise:
            if (detectDistance(-500)) {
                motor.stop();
                break;
            }
            motor.pivotCCW();
            break;

        case TurnRight:
            if (detectDistance(-400)) {
                motor.stop();
                break;
            }
            motor.turnRight(150);
            break;

        case TurnLeft:
            if (detectDistance(-400)) {
                motor.stop();
                break;
            }
            motor.turnLeft(150);
            break;

        case FollowLeft:
            motor.followLane(LEFT_EDGE, COLOR_RED, client);
            break;
            
        case FollowRight:
            motor.followLane(RIGHT_EDGE, COLOR_RED, client);
            break;

        default:
            motor.stop();
            break;
    }
}
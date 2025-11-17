#include "motorcontrol.h"
#include "state.h"
#include "linefollow.h"
#include "colorDetect.h"

void nextState(States state) 
{
  state = static_cast<States>((static_cast<int>(state) + 1) % 7);
}

void handleState(Motor &motor, States state, LineFollow lineFollow)
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
            lineFollow.followLeft(motor, COLOR_BLUE);
            break;
        case FollowRight:
            lineFollow.followRight(motor, COLOR_BLUE);
            break;

        case FollowLeftBlue:
            lineFollow.followLeft(motor, COLOR_BLUE);
            break;
        case FollowRightBlue:
            lineFollow.followRight(motor, COLOR_BLUE);
            break;
        case FollowLeftYellow:
            lineFollow.followLeft(motor, COLOR_YELLOW);
            break;
        case FollowRightYellow:
            lineFollow.followRight(motor, COLOR_YELLOW);
            break;
        case FollowLeftRed:
            lineFollow.followLeft(motor, COLOR_RED);
            break;
        case FollowRightRed:
            lineFollow.followRight(motor, COLOR_RED);
            break;

        default:
            motor.stop();
            break;
    }
}
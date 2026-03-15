/*
 *  soloDemo.cpp
 *
 *  Defining the Solo Demo Implementations
 */

#include "soloDemo.h"
#include "irDetect.h"
#include "colorDetect.h"

enum DemoState {
    DRIVE_TO_FIRST_WALL,
    PIVOT_AROUND,
    DRIVE_TO_RED,
    DRIVE_TO_BLUE,
    TURN_LEFT_AT_BLUE,
    TURN_LEFT_AT_RED,
    FOLLOW_RED_TO_WALL,
    FOLLOW_BLUE_TO_WALL,
    TURN_LEFT_AFTER_RED_WALL,
    DRIVE_TO_YELLOW,
    TURN_LEFT_AT_YELLOW,
    FOLLOW_YELLOW_TO_WALL,
    TURN_LEFT_FINAL,
    RETURN_TO_START,
    DEMO_DONE
};

/* 1. Drives Forward until it detects wall
    2. Detects wall
    3. Pivotes 170-180 degrees
    4. Drive Forward until detects Red
    5. Detects Red -> Turns left 90 Degrees
    6. Follows Red Lane until it detects wall
    7. Detects Wall -> Turns Left 90 Degrees
    8. Forward until Detects Yellow
    9. Detects Yellow -> Turn Left 90 degrees
    10. Follows Yellow Lane until it detects wall
    11. Turn Left 90 Degrees
    12. Go back to start
*/

void soloDemo(Motor &motor, WebSocketClient &client)
{
    DemoState demo = DRIVE_TO_FIRST_WALL;

    while (client.connected() && demo != DEMO_DONE)
    {
        switch (demo)
        {
            // ---------------------------------------------------------
            // 1. Drive Forward until Wall
            // ---------------------------------------------------------
            case DRIVE_TO_FIRST_WALL:
                motor.driveForward(180);
                if(detectDistance(-370)) {
                    delay(100);
                    motor.stop();
                    demo  = PIVOT_AROUND;
                }
                // motor.gyroDriveToWall(180, -370);
                // demo  = PIVOT_AROUND;
                break;

            // ---------------------------------------------------------
            // 2. Pivot 170–180 degrees
            // ---------------------------------------------------------
            case PIVOT_AROUND:
                // motor.pivotCW();
                // delay(1500);        // Tune experimentally
                // motor.stop();
                motor.gyroTurn(170);
                demo = DRIVE_TO_RED;
                break;

            // ---------------------------------------------------------
            // 3. Drive Forward until Detect Red
            // ---------------------------------------------------------
            case DRIVE_TO_RED:
            {
                motor.driveForward(180);
                int c = detectColorClass(5);
                if (c == COLOR_RED) { 
                    c = detectColorClass(5);
                    if (c == COLOR_RED) {
                        motor.stop();
                        delay(300);
                        demo = TURN_LEFT_AT_RED;
                    }
                }
                break;
            }

            // ---------------------------------------------------------
            // 4. PIVOT CounterClockwise 90°
            // ---------------------------------------------------------
            case TURN_LEFT_AT_RED:
                motor.pivotCCW();
                delay(900);
                motor.stop();
                demo = FOLLOW_RED_TO_WALL;
                break;

            // ---------------------------------------------------------
            // 5. Follow Red Lane until Wall
            // ---------------------------------------------------------
            case FOLLOW_RED_TO_WALL:
                motor.followLane(LEFT_EDGE, COLOR_RED, client);  // 3 = Red
                if (detectDistance(-300)) {
                    motor.stop();
                    demo = TURN_LEFT_AFTER_RED_WALL;
                }
                break;

            // ---------------------------------------------------------
            // 6. Pivot CCW 90°
            // ---------------------------------------------------------
            case TURN_LEFT_AFTER_RED_WALL:
                motor.pivotCCW();
                delay(900);
                motor.stop();
                demo = DRIVE_TO_YELLOW;
                break;

            // ---------------------------------------------------------
            // 7. Drive until Yellow
            // ---------------------------------------------------------
            case DRIVE_TO_YELLOW:
            {
                motor.driveForward(180);
                int c = detectColorClass(5);
                if (c == COLOR_YELLOW) {
                    c = detectColorClass(5);
                    if (c == COLOR_YELLOW) {
                        motor.stop();
                        delay(300);
                        demo = TURN_LEFT_AT_YELLOW;
                    }
                }
                break;
            }

            // ---------------------------------------------------------
            // 8. Pivot Left 90°
            // ---------------------------------------------------------
            case TURN_LEFT_AT_YELLOW:
                motor.pivotCCW();
                delay(900);
                motor.stop();
                demo = FOLLOW_YELLOW_TO_WALL;
                break;

            // ---------------------------------------------------------
            // 9. Follow Yellow until Wall
            // ---------------------------------------------------------
            case FOLLOW_YELLOW_TO_WALL:
                motor.followLane(LEFT_EDGE, COLOR_YELLOW, client);  // 3 = Red
                if (detectDistance(-300)) {
                    motor.stop();
                    demo = TURN_LEFT_FINAL;
                }
                break;

            // ---------------------------------------------------------
            // 10. Turn Left 90°
            // ---------------------------------------------------------
            case TURN_LEFT_FINAL:
                motor.pivotCCW();
                delay(900);
                motor.stop();
                demo = RETURN_TO_START;
                break;

            // ---------------------------------------------------------
            // 11. Drive back to Start
            // ---------------------------------------------------------
            case RETURN_TO_START:
                motor.driveForward(180);
                if(detectDistance(-370)) {
                    delay(100);
                    motor.stop();
                    demo  = DEMO_DONE;
                }
                break;

            default:
                demo = DEMO_DONE;
                break;
        }

        delay(10);   // loop pacing
    }
}

// Mirrored Demo
void mirrorDemo(Motor &motor, WebSocketClient &client)
{
    DemoState demo = DRIVE_TO_FIRST_WALL;

    while (client.connected() && demo != DEMO_DONE)
    {
        switch (demo)
        {
            // ---------------------------------------------------------
            // 1. Drive Forward until Wall
            // ---------------------------------------------------------
            case DRIVE_TO_FIRST_WALL:
                // motor.driveForward(180);
                // if(detectDistance(-370)) {
                //     delay(100);
                //     motor.stop();
                //     demo  = PIVOT_AROUND;
                // }
                motor.gyroDriveToWall(180, -370);
                demo  = PIVOT_AROUND;
                break;

            // ---------------------------------------------------------
            // 2. Pivot 170–180 degrees
            // ---------------------------------------------------------
            case PIVOT_AROUND:
                // motor.pivotCW();
                // delay(1500);        // Tune experimentally
                // motor.stop();
                motor.gyroTurn(170);
                demo = DRIVE_TO_BLUE;
                break;

            // ---------------------------------------------------------
            // 3. Drive Forward until Detect Blue
            // ---------------------------------------------------------
            case DRIVE_TO_BLUE:
            {
                motor.driveForward(180);
                int c = detectColorClass(5);
                if (c == COLOR_BLUE) { 
                    c = detectColorClass(5);
                    if (c == COLOR_BLUE) {
                        motor.stop();
                        delay(300);
                        demo = TURN_LEFT_AT_BLUE;
                    }
                }
                break;
            }

            // ---------------------------------------------------------
            // 4. PIVOT CounterClockwise 90°
            // ---------------------------------------------------------
            case TURN_LEFT_AT_BLUE:
                motor.pivotCCW();
                delay(900);
                motor.stop();
                demo = FOLLOW_BLUE_TO_WALL;
                break;

            // ---------------------------------------------------------
            // 5. Follow Blue Lane until Wall
            // ---------------------------------------------------------
            case FOLLOW_BLUE_TO_WALL:
                motor.followLane(LEFT_EDGE, COLOR_BLUE, client); 
                if (detectDistance(-280)) {
                    motor.stop();
                    demo = TURN_LEFT_AFTER_RED_WALL;
                }
                break;

            // ---------------------------------------------------------
            // 6. Pivot CCW 90°
            // ---------------------------------------------------------
            case TURN_LEFT_AFTER_RED_WALL:
                motor.pivotCCW();
                delay(900);
                motor.stop();
                demo = DRIVE_TO_YELLOW;
                break;

            // ---------------------------------------------------------
            // 7. Drive until Yellow
            // ---------------------------------------------------------
            case DRIVE_TO_YELLOW:
            {
                motor.driveForward(180);
                int c = detectColorClass(5);
                if (c == COLOR_YELLOW) {
                    c = detectColorClass(5);
                    if (c == COLOR_YELLOW) {
                        motor.stop();
                        delay(300);
                        demo = TURN_LEFT_AT_YELLOW;
                    }
                }
                break;
            }

            // ---------------------------------------------------------
            // 8. Pivot Left 90°
            // ---------------------------------------------------------
            case TURN_LEFT_AT_YELLOW:
                motor.pivotCCW();
                delay(900);
                motor.stop();
                demo = FOLLOW_YELLOW_TO_WALL;
                break;

            // ---------------------------------------------------------
            // 9. Follow Yellow until Wall
            // ---------------------------------------------------------
            case FOLLOW_YELLOW_TO_WALL:
                motor.followLane(LEFT_EDGE, COLOR_YELLOW, client); 
                if (detectDistance(-300)) {
                    motor.stop();
                    demo = TURN_LEFT_FINAL;
                }
                break;

            // ---------------------------------------------------------
            // 10. Turn Left 90°
            // ---------------------------------------------------------
            case TURN_LEFT_FINAL:
                motor.pivotCCW();
                delay(900);
                motor.stop();
                demo = RETURN_TO_START;
                break;

            // ---------------------------------------------------------
            // 11. Drive back to Start
            // ---------------------------------------------------------
            case RETURN_TO_START:
                motor.driveForward(180);
                if(detectDistance(-370)) {
                    delay(100);
                    motor.stop();
                    demo  = DEMO_DONE;
                }
                break;

            default:
                demo = DEMO_DONE;
                break;
        }

        delay(10);   // loop pacing
    }
}

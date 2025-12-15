/*
 *  soloDemo.cpp
 *
 *  Defining the Partner Demo Implementations
 */

#include "soloDemo.h"
#include "irDetect.h"
#include "colorDetect.h"
#include "websocket.h"

enum PartnerDemoState {
    DRIVE_TO_FIRST_WALL,
    PIVOT_AROUND,
    DRIVE_TO_RED,
    TURN_LEFT_AT_RED,
    FOLLOW_RED_TO_WALL,
    TURN_LEFT_AFTER_RED_WALL,
    DRIVE_TO_YELLOW,
    TURN_LEFT_AT_YELLOW,
    WAIT_AT_YELLOW,
    FOLLOW_YELLOW_TO_WALL,
    TURN_LEFT_FINAL,
    RETURN_TO_START,
    DEMO_DONE
};

void partnerDemo(Motor &motor, States state, WebSocketClient &client, int lineColor)
{
    PartnerDemoState demo = DRIVE_TO_FIRST_WALL;
    client.beginMessage(TYPE_TEXT);
    client.println("beep");
    client.endMessage();

    while (client.connected() && demo != DEMO_DONE)
    {
        switch (demo)
        {
            // ---------------------------------------------------------
            // 1. Drive Forward until Wall
            // ---------------------------------------------------------
            case DRIVE_TO_FIRST_WALL:
                // motor.driveForward(180);
                // if (detectDistance(-400)) {    // Wall detected
                //     motor.stop();
                //     delay(300);
                //     demo = PIVOT_AROUND;
                // }
                motor.gyroDriveToWall(180, -400);
                demo = PIVOT_AROUND;
                break;

            // ---------------------------------------------------------
            // 2. Pivot 170–180 degrees
            // ---------------------------------------------------------
            case PIVOT_AROUND:
                motor.pivotCCW();
                delay(900);        // Tune experimentally
                motor.stop();
                demo = DRIVE_TO_RED;
                break;

            // ---------------------------------------------------------
            // 3. Drive Forward until Detect Red
            // ---------------------------------------------------------
            case DRIVE_TO_RED:
            {
                motor.driveForward(180);
                int c = detectColorClass(25);
                if (c == COLOR_RED) { 
                    motor.stop();
                    delay(300);
                    demo = TURN_LEFT_AT_RED;
                    client.beginMessage(TYPE_TEXT);
                    client.println("red lane found");
                    client.endMessage();
                }
                break;
            }

            // ---------------------------------------------------------
            // 4. PIVOT CounterClockwise 90°
            // ---------------------------------------------------------
            case TURN_LEFT_AT_RED:
                motor.pivotCCW();
                delay(300);
                motor.stop();
                demo = FOLLOW_RED_TO_WALL;
                break;

            // ---------------------------------------------------------
            // 5. Follow Red Lane until Wall
            // ---------------------------------------------------------
            case FOLLOW_RED_TO_WALL:
                motor.followLane(LEFT_EDGE, COLOR_RED, client);  // 3 = Red
                if (detectDistance(-400)) {
                    motor.stop();
                    demo = TURN_LEFT_AFTER_RED_WALL;
                }
                break;

            // ---------------------------------------------------------
            // 6. Pivot CCW 90°
            // ---------------------------------------------------------
            case TURN_LEFT_AFTER_RED_WALL:
                motor.pivotCCW();
                delay(300);
                motor.stop();
                demo = DRIVE_TO_YELLOW;
                break;

            // ---------------------------------------------------------
            // 7. Drive until Yellow
            // ---------------------------------------------------------
            case DRIVE_TO_YELLOW:
            {
                motor.driveForward(180);
                int c = detectColorClass(25);
                if (c == COLOR_YELLOW) {
                    motor.stop();
                    delay(300);
                    demo = TURN_LEFT_AT_YELLOW;
                }
                break;
            }

            // ---------------------------------------------------------
            // 8. Pivot Left 90°
            // ---------------------------------------------------------
            case TURN_LEFT_AT_YELLOW:
                motor.pivotCCW();
                delay(300);
                motor.stop();
                demo = WAIT_AT_YELLOW;
                break;
            
            // ---------------------------------------------------------
            // 8.5 . Wait till Partner sends signal
            // ---------------------------------------------------------
            case WAIT_AT_YELLOW:
                if (client.parseMessage() > 0) {

                    String parsed = parseMessage(client);
                    Serial.println(parsed);
                    if (parsed.startsWith("PARTNER:")) {
                        String command = parsed.substring(8); // strip "PARTNER:"
                    }
                }
                break;

            // ---------------------------------------------------------
            // 9. Follow Yellow until Wall
            // ---------------------------------------------------------
            case FOLLOW_YELLOW_TO_WALL:
                motor.followLane(LEFT_EDGE, COLOR_YELLOW, client);  
                if (detectDistance(-200)) {
                    motor.stop();
                    demo = TURN_LEFT_FINAL;
                }
                break;

            // ---------------------------------------------------------
            // 10. Turn Left 90°
            // ---------------------------------------------------------
            case TURN_LEFT_FINAL:
                motor.turnLeft(150);
                delay(300);
                motor.stop();
                demo = RETURN_TO_START;
                break;

            // ---------------------------------------------------------
            // 11. Drive back to Start
            // ---------------------------------------------------------
            case RETURN_TO_START:
                motor.driveForward(180);
                if (detectDistance(-400)) {  // Assume start has a wall
                    motor.stop();
                    while(1) {
                        client.beginMessage(TYPE_TEXT);
                        client.println("State: Done");
                        client.endMessage();
                        delay(1000);
                    }                    
                    demo = DEMO_DONE;
                }
                break;

            default:
                demo = DEMO_DONE;
                break;
        }

        delay(10);   // loop pacing
    }
}

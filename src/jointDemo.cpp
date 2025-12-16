/*
 *  jointDemo.cpp
 *
 *  Defining the Solo Demo Implementations
 */

#include "soloDemo.h"
#include "irDetect.h"
#include "colorDetect.h"
#include "websocket.h"

enum JointState {
    WAIT_FOR_RED,
    DRIVE_TO_FIRST_WALL,
    PIVOT_AROUND,
    DRIVE_TO_RED,
    DRIVE_TO_BLUE,
    TURN_LEFT_AT_RED,
    TURN_LEFT_AT_BLUE,
    FOLLOW_RED_TO_WALL,
    FOLLOW_BLUE_TO_WALL,
    TURN_LEFT_AFTER_RED_WALL,
    TURN_LEFT_AFTER_BLUE_WALL,
    WAIT_FOR_YELLOW,
    DRIVE_TO_YELLOW,
    TURN_LEFT_AT_YELLOW,
    WAIT_FOR_BLUE,
    FOLLOW_YELLOW_TO_WALL,
    TURN_LEFT_FINAL,
    RETURN_TO_START,
    DEMO_DONE,
    WAIT_AT_YELLOW
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
    9b. Wait for Blue to be received by other bot
    10. Follows Yellow Lane until it detects wall
    11. Turn Left 90 Degrees
    12. Go back to start
*/

void jointDemo(Motor &motor, WebSocketClient &client)
{
    JointState demo = DRIVE_TO_FIRST_WALL; //WAIT_FOR_RED;

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
                        client.beginMessage(TYPE_TEXT);
                        client.print("State: Red");
                        client.endMessage();

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
                motor.followLane(LEFT_EDGE, COLOR_RED, client);  
                if (detectDistance(-300)) {
                    motor.stop();
                    demo = TURN_LEFT_AFTER_RED_WALL;
                }
                break;

            // ---------------------------------------------------------
            // 6. Pivot CCW 90°
            // ---------------------------------------------------------
            case TURN_LEFT_AFTER_RED_WALL:
                // motor.pivotCCW();
                // delay(900);
                // motor.stop();
                motor.gyroTurn(90);
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
                demo = WAIT_FOR_BLUE;
                break;

            // ---------------------------------------------------------
            // 9. Wait for Blue Command
            // --------------------------------------------------------- 
            case WAIT_FOR_BLUE:
            {
                if (client.parseMessage() > 0) {
                    String parsed = parseMessage(client);
                    Serial.println(parsed);

                    if (parsed.startsWith("PARTNER:")) {
                        String command = parsed.substring(8);
                        command.trim();

                        if (command == "State: Blue") {
                            demo = FOLLOW_YELLOW_TO_WALL;
                        }
                    }
                }
                break;
            }


            // ---------------------------------------------------------
            // 10. Follow Yellow until Wall
            // ---------------------------------------------------------
            case FOLLOW_YELLOW_TO_WALL:
                client.beginMessage(TYPE_TEXT);
                client.print("following yellow");
                client.endMessage();
                motor.followLane(LEFT_EDGE, COLOR_YELLOW, client); 
                if (detectDistance(-400)) {
                    motor.stop();
                    demo = TURN_LEFT_FINAL;
                }
                break;

            // ---------------------------------------------------------
            // 11. Turn Left 90°
            // ---------------------------------------------------------
            case TURN_LEFT_FINAL:
                motor.pivotCCW();
                delay(900);
                motor.stop();
                demo = RETURN_TO_START;
                break;

            // ---------------------------------------------------------
            // 12. Drive back to Start
            // ---------------------------------------------------------
            case RETURN_TO_START:
                motor.driveForward(180);
                if(detectDistance(-370)) {
                    delay(100);
                    motor.stop();
                    while (1) {
                        client.beginMessage(TYPE_TEXT);
                        client.print("State: Done");
                        client.endMessage();
                        delay(2000);
                    }
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

void mirrorJointDemo(Motor &motor, WebSocketClient &client)
{
    JointState demo = WAIT_FOR_RED;

    while (client.connected() && demo != DEMO_DONE)
    {
        switch (demo)
        {
            // ---------------------------------------------------------
            // 0. Wait for Red From Other Group
            // ---------------------------------------------------------
            case WAIT_FOR_RED:
            {
                if (client.parseMessage() > 0) {
                    String parsed = parseMessage(client);
                    Serial.println(parsed);

                    if (parsed.startsWith("PARTNER:")) {
                        String command = parsed.substring(8);
                        command.trim();

                        if (command == "State: Red") {
                            delay(2500); // Make sure DEI is out of the way
                            demo = DRIVE_TO_FIRST_WALL;
                        }
                    }
                }
                break;
            }
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
                motor.gyroTurn(150);
                demo = DRIVE_TO_BLUE;
                break;

            // ---------------------------------------------------------
            // 3. Drive Forward until Detect Red
            // ---------------------------------------------------------
            case DRIVE_TO_BLUE:
            {
                motor.driveForward(180);
                int c = detectColorClass(5);
                if (c == COLOR_BLUE) { 
                    c = detectColorClass(5);
                    if (c == COLOR_BLUE) {
                        client.beginMessage(TYPE_TEXT);
                        client.print("State: Blue");
                        client.endMessage();

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
                motor.pivotCW();
                delay(900);
                motor.stop();
                delay(2500);
                demo = WAIT_FOR_YELLOW;
                break;

            // ---------------------------------------------------------
            // 5. Follow Blue Lane until Wall
            // ---------------------------------------------------------
            case WAIT_FOR_YELLOW:
                client.beginMessage(TYPE_TEXT);
                client.print("State: Blue");
                client.endMessage();
                if (client.parseMessage() > 0) {
                    String parsed = parseMessage(client);
                    Serial.println(parsed);

                    if (parsed.startsWith("PARTNER:")) {
                        String command = parsed.substring(8);
                        command.trim();

                        if (command == "State: Yellow") {
                            delay(1000); // Make sure DEI is out of the way
                            demo = FOLLOW_BLUE_TO_WALL;
                        }
                    }
                }
                break;
                
            
            case FOLLOW_BLUE_TO_WALL:
                motor.followLane(RIGHT_EDGE, COLOR_BLUE, client);  
                if (detectDistance(-300)) {
                    motor.stop();
                    demo = TURN_LEFT_AFTER_BLUE_WALL;
                }
                break;

            // ---------------------------------------------------------
            // 6. Pivot CCW 90°
            // ---------------------------------------------------------
            case TURN_LEFT_AFTER_BLUE_WALL:
                motor.pivotCW();
                delay(900);
                motor.stop();
                demo = DRIVE_TO_YELLOW;
                break;
            
            // ---------------------------------------------------------
            // 6.5 Pivot CCW 90°
            // ---------------------------------------------------------
            case WAIT_AT_YELLOW:
            {
                if (client.parseMessage() > 0) {
                    String parsed = parseMessage(client);
                    Serial.println(parsed);

                    if (parsed.startsWith("PARTNER:")) {
                        String command = parsed.substring(8);
                        command.trim();

                        if (command == "State: Done") {
                            demo = FOLLOW_YELLOW_TO_WALL;
                        }
                    }
                }
                break;
            }

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
                // motor.pivotCW();
                // delay(900);
                // motor.stop();
                motor.gyroTurn(-75);
                demo = WAIT_AT_YELLOW;
                break;

            // ---------------------------------------------------------
            // 10. Follow Yellow until Wall
            // ---------------------------------------------------------
            case FOLLOW_YELLOW_TO_WALL:
                motor.followLane(RIGHT_EDGE, COLOR_YELLOW, client); 
                if (detectDistance(-400)) {
                    motor.stop();
                    demo = TURN_LEFT_FINAL;
                }
                break;

            // ---------------------------------------------------------
            // 11. Turn Left 90°
            // ---------------------------------------------------------
            case TURN_LEFT_FINAL:
                motor.pivotCW();
                delay(900);
                motor.stop();
                demo = RETURN_TO_START;
                break;

            // ---------------------------------------------------------
            // 12. Drive back to Start
            // ---------------------------------------------------------
            case RETURN_TO_START:
                motor.driveForward(180);
                if(detectDistance(-370)) {
                    delay(100);
                    motor.stop();
                    while(1) {
                        client.beginMessage(TYPE_TEXT);
                        client.print("State: Done");
                        client.endMessage();
                        delay(1000);
                    }

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

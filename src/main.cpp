#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>
#include "secrets.h"
#include "websocket.h"
#include "motorcontrol.h"
#include "state.h"
#include "colorDetect.h"
#include "soloDemo.h"
#include "irDetect.h"

/* SERVER CONFIGURATION */
// wscat -c ws://10.5.12.14

/////// you can enter your sensitive data in the Secret tab/arduino_secrets.h
/////// WiFi Settings ///////
char ssid[] = WIFI_NAME;
char pass[] = WIFI_PASSWORD;
char serverAddress[] = ADDRESS; // server address
int port = 8080;
// 80 - Josh
// 8080 - Halligan

WiFiClient wifi;
WebSocketClient client = WebSocketClient(wifi, serverAddress, port);
String clientID = CLIENT_ID; 
States state = STOP;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(A0, INPUT_PULLUP);

    colorDetectSetup();
    initializeWifi(ssid, pass);
    irDetectSetup(100);
    client.begin();
}

int current_blue, current_yellow, current_red, current_class = 0;
Motor motor;

void remoteCommanBotMotionsWithPartner() 
{
    // while (client.connected()) {
    //     if (client.parseMessage() > 0) {

    //         String parsed = parseMessage(client);
    //         Serial.println(parsed);
    //         if (parsed.startsWith("PARTNER:")) {

    //             String command = parsed.substring(8); // strip "PARTNER:"
    //             if (command == "State: 1") {
    //                 handleState(motor, (States) 1, client);
    //                 delay(5000);
    //                 handleState(motor, (States) 0, client);

    //                 /* Flash LED*/
    //                 digitalWrite(LED_BUILTIN, HIGH);
    //                 delay(300);
    //                 digitalWrite(LED_BUILTIN, LOW);

    //                 client.beginMessage(TYPE_TEXT);
    //                 client.print("State: 2");
    //                 client.endMessage();
    //                 Serial.println("MOVE sent to server");
    //             }
    //         }
    //     }
    // }
}

void loop() 
{
    
    Serial.println("Done Connecting");
    client.beginMessage(TYPE_TEXT);
    client.print(clientID);
    client.endMessage();

    //soloDemo(motor, client);
    
    while (true) {
        detectDistance(10000);
        delay(500);
    }
    // while( true ) {
    //     state = (States) FollowLeft;
    //     handleState(motor, state, client, COLOR_YELLOW);     
    // }

    Serial.println("disconnected");

    while (client.connected()) {
        if (client.parseMessage() > 0) {
            /* Read Message Constantly from the Server, only from our bot / DEI */
            String parsed = parseMessage(client);
            String command;

            if (parsed != "") {
                client.beginMessage(TYPE_TEXT);
                client.println(parsed + " Hoang Mai");
                client.endMessage();    
            }

            /* Parse Message from Websocket depending on who sent it */
            if (parsed.startsWith("PARTNER:")) {
                command = parsed.substring(8); // strip "PARTNER:"
            } else if (parsed.startsWith("SELF:")) {
                command = parsed.substring(5);
            }
            
            /* Implement the State Logic */
            int newState   = parseState(command);
            if (newState >= STOP && newState <= TurnLeft) {
                state = (States) newState;
                Serial.print("Server set state to: ");
                Serial.println(state);
            }
        }
    }
}


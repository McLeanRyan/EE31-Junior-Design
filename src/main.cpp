#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>
#include "secrets.h"
#include "websocket.h"
#include "motorcontrol.h"
#include "state.h"
#include "colorDetect.h"

/* SERVER CONFIGURATION */
// wscat -c ws://10.5.12.14

/////// you can enter your sensitive data in the Secret tab/arduino_secrets.h
/////// WiFi Settings ///////
char ssid[] = WIFI_NAME;
char pass[] = WIFI_PASSWORD;
char serverAddress[] = ADDRESS; // server address
int port = 80;

WiFiClient wifi;
WebSocketClient client = WebSocketClient(wifi, serverAddress, port);
String clientID = CLIENT_ID; //Insert your Server ID Here!
int status = WL_IDLE_STATUS;

volatile bool buttonPressed = false;  // set in ISR
States state = STOP;

// ISR
void handleButtonInterrupt() {
  buttonPressed = true;
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(A0, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(A0), handleButtonInterrupt, FALLING);

    colorDetectSetup();

    initializeWifi(ssid, pass, status);
}

int current_blue, current_yellow, current_red, current_class = 0;

Motor motor;

// Both bots are next to each other and not moving.
// Bot 1 flashes their Arduino LED.
// Bot 1 communicates to Bot 2 to move forward for five seconds. 
// Bot 2 receives the message and move forward for five seconds then stops.
// When Bot 2 is finished moving forward, it flashes it's built-in LED. 
// Bot 2 communicates to Bot 1 to move forward for five seconds. 
// Bot 1 receives the signal and moves forward for five seconds then stop.

void remoteCommanBotMotionsWithPartner() {
    while (client.connected()) {
        if (client.parseMessage() > 0) {

            String parsed = joshParseMessage(client);
            Serial.println(parsed);
            if (parsed.startsWith("PARTNER:")) {

                String command = parsed.substring(8); // strip "PARTNER:"
                if (command == "State: 1") {
                    handleState(motor, (States) 1);
                    delay(5000);
                    handleState(motor, (States) 0 );

                    /* Flash LED*/
                    digitalWrite(LED_BUILTIN, HIGH);
                    delay(300);
                    digitalWrite(LED_BUILTIN, LOW);

                    client.beginMessage(TYPE_TEXT);
                    client.print("State: 2");
                    client.endMessage();
                    Serial.println("MOVE sent to server");
                }
            }
        }
    }
}

void loop() {
    delay(100);

    if (!client.connected()) {
        client.begin();
        delay(100);
        client.beginMessage(TYPE_TEXT);
        client.print(clientID);
        client.endMessage();
    }

    //remoteCommanBotMotionsWithPartner();
    
    while (client.connected()) {
        if (client.parseMessage() > 0) {
            /* Read Message Constantly from the Server, only from our bot / DEI */
            String parsed = joshParseMessage(client);
            String command;

            client.beginMessage(TYPE_TEXT);
            client.println("Received message: " + parsed);
            client.endMessage();

            // /* Parse Message from Websocket depending on who sent it */
            // if (parsed.startsWith("PARTNER:")) {
            //     command = parsed.substring(8); // strip "PARTNER:"
            // } else if (parsed.startsWith("SELF:")) {
            //     command = parsed.substring(5);
            // }
            
            // /* Implement the State Logic */
            // int newState   = parseState(command);
            // if (newState >= STOP && newState <= TurnLeft) {
            //     state = (States) newState;
            //     Serial.print("Server set state to: ");
            //     Serial.println(state);
            // }
        }
        handleState(motor, state);
    }

    Serial.println("disconnected");
}
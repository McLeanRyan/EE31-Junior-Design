#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>
#include "secrets.h"
#include "websocket.h"
#include "motorcontrol.h"
#include "state.h"
#include "colorDetect.h"
#include "linefollow.h"

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
LineFollow lineFollow;

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

void loop() {
    delay(100);

    while(true){
        state = (States) FollowLeft;
        handleState(motor, state, lineFollow); 
        //detectColorClass(50);
        
    }

    if (!client.connected()) {
        client.begin();
        delay(100);
        client.beginMessage(TYPE_TEXT);
        client.print(clientID);
        client.endMessage();
    }

    while (client.connected()) {
        int messageSize = client.parseMessage();
        if (messageSize > 0) {

            String message = parseMessage(client);
            Serial.println("Message received");
            Serial.println(message);
            int newState   = parseState(message);
            Serial.println(newState);
            if (newState >= STOP && newState <= TurnLeft) {
                state = (States) FollowLeft;
                Serial.print("Server set state to: ");
                Serial.println(state);

                client.beginMessage(TYPE_TEXT);
                client.print("state updated to ");
                client.print(state);
                client.endMessage();
                Serial.println("sent to server");
            }
        }

        // if (buttonPressed) {
        //     buttonPressed = false; // clear flag
        //     nextState();
        //     Serial.print("State changed to: ");
        //     Serial.println(state);
        // }
    
        // if (Serial.available()) {
        //     String input = Serial.readStringUntil('\n');
        //     client.beginMessage(TYPE_TEXT);
        //     client.print(input);
        //     client.endMessage();
        //     Serial.print("Sent from Serial: ");
        //     Serial.println(input);
        // }

        // Repeatedly blink to indicate current state 
        // int blinkCount = state + 1;

        // for (int i = 0; i < blinkCount; i++) {
        //     digitalWrite(LED_BUILTIN, HIGH);
        //     delay(250);
        //     digitalWrite(LED_BUILTIN, LOW);
        //     delay(250);
        // }

        handleState(motor, state, lineFollow);
    }

    Serial.println("disconnected");
}
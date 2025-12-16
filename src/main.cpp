#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>
#include "secrets.h"
#include "websocket.h"
#include "motorcontrol.h"
#include "state.h"
#include "colorDetect.h"

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

    initializeWifi(ssid, pass);
    irDetectSetup(100);
    // client.begin();
    imuSetup();

}

int current_blue, current_yellow, current_red, current_class = 0;

Motor motor;

void loop() {
    delay(100);

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
                state = (States) newState;
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
    
    Serial.println("Done Connecting");
    // client.beginMessage(TYPE_TEXT);
    // client.print(clientID);
    // client.endMessage();
    // client.beginMessage(TYPE_TEXT);
    // client.print("Server Connected!");
    // client.endMessage();
    // while(true) {
    //     detectColorClass(5);
    // }
    
    // client.beginMessage(TYPE_TEXT);
    // client.print("State: Red");
    // client.endMessage();
    // delay(30000);
    // while(1){
    //     // Serial.println(detectColorClass());
        
    //     client.beginMessage(TYPE_TEXT);
    //     client.print("State: Done");
    //     client.endMessage();
    //     delay(1000);
    // }

    mirrorDemo(motor, client);
    // jointDemo(motor, client);

        // for (int i = 0; i < blinkCount; i++) {
        //     digitalWrite(LED_BUILTIN, HIGH);
        //     delay(250);
        //     digitalWrite(LED_BUILTIN, LOW);
        //     delay(250);
        // }

        handleState(motor, state);
    }

    Serial.println("disconnected");
}
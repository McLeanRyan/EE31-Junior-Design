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

    soloDemo(motor, client);
    
    // while( true ) {
    //     state = (States) FollowLeft;
    //     handleState(motor, state, client, COLOR_YELLOW);     
    // }

    messageState(client);

    Serial.println("disconnected");

}


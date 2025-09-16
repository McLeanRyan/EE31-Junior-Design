#include <ArduinoHttpClient.h>
#include <WiFi.h>
#include "secrets.h"
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
int count = 0;

void setup() {
    Serial.begin(9600);
    while ( status != WL_CONNECTED) {
        Serial.print("Attempting to connect to Network named: ");
        Serial.println(ssid); // print the network name (SSID);

        // Connect to WPA/WPA2 network:
        status = WiFi.begin(ssid, pass);
    }

    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
}

void loop() {
    // start Websocket Client
    Serial.println("starting WebSocket client");
    client.begin();
    client.beginMessage(TYPE_TEXT);
    client.print(clientID);
    client.endMessage();

    while (client.connected()) {
    }

    Serial.println("disconnected");
}

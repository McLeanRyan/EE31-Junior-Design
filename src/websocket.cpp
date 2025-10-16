/*
 *  websocket.cpp
 *  Hoang Mai
 *  10/02/2025
 *
 *
 *  Initializes and sets up the functionalities of wifi and server communication
 */
#include "websocket.h"
#include <WiFiNINA.h>

void initializeWifi(char ssid[], char pass[], int status)
{
    Serial.begin(9600);
    while ( status != WL_CONNECTED) {
        Serial.print("Attempting to connect to Network named: ");
        Serial.println(ssid); // print the network name (SSID);

        // Connect to WPA/WPA2 network:
        status = WiFi.begin(ssid, pass);
    }
    Serial.print("Done\n");
}

String parseMessage(WebSocketClient& client) {
    String message = client.readString();
    message.trim();

    // Filter out non-printable characters (protects from binary garbage)
    String filtered = "";
    for (int i = 0; i < message.length(); i++) {
        if (isPrintable(message[i])) {
            filtered += message[i];
        }
    }
    return filtered;
}

int parseState(String message)
{
    char lastChar = ' ';
    Serial.println(message);

    if (message.indexOf("WebClient_F79721857DC5") == -1) return -1;
    if (message.length() == 0) return -1;

    lastChar = message.charAt(message.length() - 1);
    int newState = lastChar - '0';
    return newState;
}

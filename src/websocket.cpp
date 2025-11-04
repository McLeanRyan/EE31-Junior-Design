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
    const String MY_ID      = "WebClient_F79721857DC5";
    const String PARTNER_ID = "56FC703ACE1A";
    
    // Read and clean incoming message
    String raw = client.readString();
    raw.trim();
    String message = "";
    for (char c : raw) {
        if (isPrintable(c)) message += c;
    }

<<<<<<< Updated upstream
    // Only process messages from *either* known bot IDs
    if (message.startsWith(PARTNER_ID)) {
        int dotIndex = message.indexOf('.');
        if (dotIndex != -1) {
            String content = message.substring(dotIndex + 1);
            Serial.println("Received from partner: " + content);
            return "PARTNER:" + content;
        }
    } 
    else if (message.startsWith(MY_ID)) {
        int dotIndex = message.indexOf('.');
        if (dotIndex != -1) {
            String content = message.substring(dotIndex + 1);
            Serial.println("Received from self: " + content);
            return "SELF:" + content;
        }
    }

    // Ignore all other sources
    return "";
=======
    String otherBot = "56FC703ACE1A_7210";
    String ourBot   = "WebClient_F79721857DC5";

    // Filter out non-printable characters (protects from binary garbage)
    String filtered = "";
    for (int i = 0; i < message.length(); i++) {
        if (isPrintable(message[i])) {
            filtered += message[i];
        }
    }

    /* Parse other bot's message */
    if (message.startsWith(otherBot)) {
       int dotIndex = message.indexOf('.');
       if (dotIndex != -1) {
            // Extract everything after the dot
            filtered = "Other Bot" + message.substring(dotIndex + 1);
            Serial.println(filtered);
        }
    }

    return filtered;
>>>>>>> Stashed changes
}

int parseState(String message)
{
    char lastChar = ' ';
    lastChar = message.charAt(message.length() - 1);
    int newState = lastChar - '0';
    return newState;
}   
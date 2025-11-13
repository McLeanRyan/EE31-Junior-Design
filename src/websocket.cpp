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

/*  initializeWifi
    Description:    Initialize connecting to the wifi
    Input:          Wifi Name, Password, Connection Status
    Output:         None
*/
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

/*  parseMessage
    Description:    Only Read the Messages sent from our Bot and Partner's Bot
    Input:          Client Object
    Output:         The message after parsing
*/
String parseMessage(WebSocketClient& client) {
    const String MY_ID      = "WebClient_F79721857DC5";
    const String PARTNER_ID = "56FC703ACE1A";
    
    /* Increment only readable characters from the websocket */
    String raw = client.readString();
    raw.trim();
    String message = "";
    for (char c : raw) {
        if (isPrintable(c)) message += c;
    }

    /* Only process messages from *either* known bot IDs */
    if (message.startsWith(PARTNER_ID)) {
        int dotIndex = message.indexOf('.');
        if (dotIndex != -1) {
            String content = message.substring(dotIndex + 1);
            Serial.println("Received from partner: " + content);
            return "PARTNER:" + content;
        }
    } else if (message.startsWith(MY_ID)) {
        int dotIndex = message.indexOf('.');
        if (dotIndex != -1) {
            String content = message.substring(dotIndex + 1);
            Serial.println("Received from self: " + content);
            return "SELF:" + content;
        }
    }

    // Ignore all other sources
    return "";
}

String joshParseMessage(WebSocketClient &client)
{
    const String PARTNERBOT_ID = "56FC703ACE1A_9664";
    const String MY_ID = "Chroma";

    String raw = client.readString();
    raw.trim();
    String message = "";
    for (char c : raw) {
        if (isPrintable(c)) message += c;
    }

    /* Only process messages from *either* known bot IDs */
    if (message.startsWith(PARTNERBOT_ID)) {
        int dotIndex = message.indexOf(',');
        if (dotIndex != -1) {
            String content = message.substring(dotIndex + 1);
            Serial.println("Received from partner: " + content);
            return "PARTNER:" + content;
        }
    } else if (message.startsWith(MY_ID)) {
        int dotIndex = message.indexOf('.');
        if (dotIndex != -1) {
            String content = message.substring(dotIndex + 1);
            Serial.println("Received from self: " + content);
            return "SELF:" + content;
        }
    }
}

/*  parseState
    Description:    Read the State from a given message from the Server
    Input:          Message Read in
    Output:         Integer of State
*/
int parseState(String message)
{
    char lastChar = ' ';
    lastChar = message.charAt(message.length() - 1);
    int newState = lastChar - '0';
    return newState;
}   
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
#include "state.h"

/*  initializeWifi
    Description:    Initialize connecting to the wifi
    Input:          Wifi Name, Password, Connection Status
    Output:         None
*/
void initializeWifi(char ssid[], char pass[])
{
    Serial.begin(9600);
    int status = WL_IDLE_STATUS;

    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to Network named: ");

        Serial.println(ssid);                   
        status = WiFi.begin(ssid, pass);
    }
    Serial.print("Done\n");
}

void messageState(WebSocketClient &client)
{
    States state = STOP;
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



String parseMessage(WebSocketClient &client)
{
    const String PARTNERBOT_ID = "56FC703ACE1A";
    const String MY_ID = "Chroma";

    String raw = client.readString();
    raw.trim();
    String message = "";
    for (char c : raw) {
        if (isPrintable(c)) message += c;
    }
    Serial.println("Read: ");
    Serial.println(message);

    /* Only process messages from *either* known bot IDs */
    if (message.startsWith(PARTNERBOT_ID)) {
        int dotIndex = message.indexOf(',');
        if (dotIndex != -1) {
            String content = message.substring(dotIndex + 1);
            Serial.println("Received from partner: " + content);
            return "PARTNER:" + content;
        }
    } else if (message.startsWith(MY_ID)) {
        int dotIndex = message.indexOf(',');
        if (dotIndex != -1) {
            String content = message.substring(dotIndex + 1);
            Serial.println("Received from self: " + content);
            return "SELF:" + content;
        }
    }

    return "";
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
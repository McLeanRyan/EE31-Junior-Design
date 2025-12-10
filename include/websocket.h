#ifndef WEB_SOCKET_H
#define WEB_SOCKET_H
#include <ArduinoHttpClient.h>

void initializeWifi(char ssid[], char pass[]);
String parseMessage(WebSocketClient &client);
int parseState(String message);
void messageState(WebSocketClient &client);

#endif
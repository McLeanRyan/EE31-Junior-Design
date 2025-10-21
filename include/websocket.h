#ifndef WEB_SOCKET_H
#define WEB_SOCKET_H
#include <ArduinoHttpClient.h>
 

void initializeWifi(char ssid[], char pass[], int status);
String parseMessage(WebSocketClient &client);
int parseState(String message);

#endif
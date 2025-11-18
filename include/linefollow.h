#ifndef LINEFOLLOW_H
#define LINEFOLLOW_H

using namespace std;
#include "motorcontrol.h"
#include <ArduinoHttpClient.h>

class LineFollow {
    
public:
    void followLeft(Motor& motor, int lineColor, WebSocketClient &client);
    void followRight(Motor& motor, int lineColor, WebSocketClient &client);
private:
};

#endif
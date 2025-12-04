/*
 *  soloDemo.h
 *
 *
 *  Defining the solo Demo logic
 */
#ifndef SOLO_DEMO_H
#define SOLO_DEMO_H
#include "motorcontrol.h"
#include <ArduinoHttpClient.h>
#include "state.h"

void soloDemo(Motor &motor, States state, WebSocketClient &client, int lineColor);
    
#endif
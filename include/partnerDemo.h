/*
 *  partnerDemo.h
 *
 *
 *  Defining the solo Demo logic
 */
#ifndef PARTNER_DEMO_H
#define PARTNER_DEMO_H
#include "motorcontrol.h"
#include <ArduinoHttpClient.h>
#include "state.h"

void partnerDemo(Motor &motor, States state, WebSocketClient &client, int lineColor);
    
#endif
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

void soloDemo(Motor &motor, WebSocketClient &client);
void mirrorDemo(Motor &motor, WebSocketClient &client);

#endif
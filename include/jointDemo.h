/*
 *  jointDemo.h
 *
 *
 *  Defining the solo Demo logic
 */
#ifndef JOINT_DEMO_H
#define JOINT_DEMO_H
#include "motorcontrol.h"
#include <ArduinoHttpClient.h>
#include "state.h"

void jointDemo(Motor &motor, WebSocketClient &client);
void mirrorJointDemo(Motor &motor, WebSocketClient &client);

#endif
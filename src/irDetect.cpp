#include "irDetect.h"
#include "motorcontrol.h"
#include <Arduino.h>

#define PIN_SENSOR A3
#define PIN_IR_LED A2

static int ambient;

void irDetectSetup() {
    pinMode(PIN_IR_LED, OUTPUT);
    digitalWrite(PIN_IR_LED, LOW);
    ambient = analogRead(PIN_SENSOR);
    delay(100);
    digitalWrite(PIN_IR_LED, HIGH);
}

// returns true if distance is beyond threshold (bot should stop)
// false otherwise
bool detectDistance(int threshold) {
    int current_distance = analogRead(PIN_SENSOR) - ambient;
    Serial.println(current_distance);
    
    return current_distance > threshold;
}
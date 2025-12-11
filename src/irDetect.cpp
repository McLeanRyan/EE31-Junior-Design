#include "irDetect.h"
#include "motorcontrol.h"
#include <Arduino.h>

#define PIN_SENSOR A3
#define PIN_IR_LED A2

static int ambient;

void irDetectSetup(int delayMs) {
    pinMode(PIN_IR_LED, OUTPUT);
    digitalWrite(PIN_IR_LED, LOW);

    delay(50); // settle

    long sum = 0;
    for(int i = 0; i < 10; i++){
        sum += analogRead(PIN_SENSOR);
        delay(5);
    }
    ambient = sum / 10;
    Serial.println("Ambient is :");
    Serial.println(ambient);
    

    digitalWrite(PIN_IR_LED, HIGH);
}

// returns true if distance is beyond threshold (bot should stop)
// false otherwise
bool detectDistance(int threshold) {
    int current_distance = analogRead(PIN_SENSOR) - ambient;

    // Serial.println(current_distance); // Debug Statement

    return current_distance < threshold; //distance readings are negative
}

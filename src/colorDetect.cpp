/*
 *  colorDetect.cpp
 *  10/30/2025
 *
 *
 *  Color Detection Implementations 
 */
#include "colorDetect.h"
#include <Arduino.h>
#include <math.h>

// Pin configuration
#define PIN_RED_LED    13
#define PIN_BLUE_LED   12
#define PIN_YELLOW_LED 11
#define PIN_SENSOR     A5 // analog input

// Ambient baseline captured at setup
static int ambientBaseline = 0;

int training[4][4] = 
{
    {-35,  -50,    0, COLOR_BLACK},
    {-170, -180,   0, COLOR_BLUE},
    {-110, -100, -50, COLOR_YELLOW},
    {-70,  -60,  -20, COLOR_RED}
};

void colorDetectSetup()
{
    pinMode(PIN_RED_LED, OUTPUT);
    pinMode(PIN_BLUE_LED, OUTPUT);
    pinMode(PIN_YELLOW_LED, OUTPUT);
    digitalWrite(PIN_RED_LED, LOW);
    digitalWrite(PIN_BLUE_LED, LOW);
    digitalWrite(PIN_YELLOW_LED, LOW);

    // Capture ambient once at startup
    ambientBaseline = analogRead(PIN_SENSOR);
}

// Perform LED measurement sequence and classify
int detectColorClass(int delayMs)
{
    int current_blue = 0;
    int current_yellow = 0;
    int current_red = 0;

    // RED
    digitalWrite(PIN_RED_LED, HIGH);
    delay(delayMs);
    current_red = analogRead(PIN_SENSOR) - ambientBaseline;
    digitalWrite(PIN_RED_LED, LOW);

    // BLUE
    digitalWrite(PIN_BLUE_LED, HIGH);
    delay(delayMs);
    current_blue = analogRead(PIN_SENSOR) - ambientBaseline;
    digitalWrite(PIN_BLUE_LED, LOW);

    // YELLOW
    digitalWrite(PIN_YELLOW_LED, HIGH);
    delay(delayMs);
    current_yellow = analogRead(PIN_SENSOR) - ambientBaseline;
    digitalWrite(PIN_YELLOW_LED, LOW);

    return calculateClass(current_blue, current_yellow, current_red);
}
int calculateClass(int blue1, int yellow1, int red1) {
    int distance = 1000000;
    int temp_distance = 0;
    int blue2 = 0;
    int yellow2 = 0;
    int red2 = 0;
    int classifier = 0;

    for (int i = 0; i < 4; i++) {
        blue2 = training[i][0];
        yellow2 = training[i][1];
        red2 = training[i][2];
        temp_distance = pow((pow((blue1 - blue2), 2)) + 
                        pow((yellow1 - yellow2), 2) + pow((red1 - red2), 2), 0.5);
        
        if (temp_distance < distance) {
            distance = temp_distance;
            classifier = training[i][3];
        } 
    }
    return classifier;
}
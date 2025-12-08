/*
 *  colorDetect.cpp
 *  Romil Shah
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
    {-60,   0,     0, COLOR_BLACK},
    {-235, -0,   0, COLOR_BLUE},
    {-80,   -20, -70, COLOR_YELLOW},
    {-60,   0,   -50, COLOR_RED}
};

int med_blue[5];
int med_yellow[5];
int med_red[5];

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

    for (int i = 0; i < 5; i++) {
        // RED
        digitalWrite(PIN_RED_LED, HIGH);
        delay(delayMs);
        current_red = analogRead(PIN_SENSOR) - ambientBaseline;
        digitalWrite(PIN_RED_LED, LOW);
        med_red[i] = current_red;

        // BLUE
        digitalWrite(PIN_BLUE_LED, HIGH);
        delay(delayMs);
        current_blue = analogRead(PIN_SENSOR) - ambientBaseline;
        digitalWrite(PIN_BLUE_LED, LOW);
        med_blue[i] = current_blue;

        // YELLOW
        digitalWrite(PIN_YELLOW_LED, HIGH);
        delay(delayMs);
        current_yellow = analogRead(PIN_SENSOR) - ambientBaseline;
        digitalWrite(PIN_YELLOW_LED, LOW);
        med_yellow[i] = current_yellow;
    }

    // find median of each color reading
    current_blue   = median5(med_blue[0], med_blue[1], med_blue[2], med_blue[3], med_blue[4]);
    current_yellow = median5(med_yellow[0], med_yellow[1], med_yellow[2], med_yellow[3], med_yellow[4]);
    current_red    = median5(med_red[0], med_red[1], med_red[2], med_red[3], med_red[4]);

    return calculateClass(current_blue, current_yellow, current_red);
}

int median5(int a, int b, int c, int d, int e) {
    int t;

    // ensure a ≤ b
    if (a > b) { t = a; a = b; b = t; }

    // ensure c ≤ d
    if (c > d) { t = c; c = d; d = t; }

    // ensure a ≤ c
    if (a > c) { t = a; a = c; c = t; }

    // ensure b ≤ d
    if (b > d) { t = b; b = d; d = t; }

    // now median is between b, c, e
    if (e < b) return b;
    else if (e > c) return c;
    else return e;
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
    // Serial.println(classifier);
    return classifier;
}
/*
 *  colorDetect.cpp
 *  Hoang Mai
 *  10/16/2025
 *
 *
 *  Color Detection Implementations 
 */
#include "colorDetect.h"
#include <math.h>

#define BLACK 0 
#define BLUE 1
#define YELLOW 2
#define RED 3

int training[4][4] = 
{
    {-35, -50, 0, BLACK},
    {-170, -180, 0, BLUE},
    {-110, -100, -50, YELLOW},
    {-70, -60, -20, RED}
};

void colorDetectSetup()
{
    
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
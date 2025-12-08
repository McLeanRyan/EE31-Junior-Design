/*
 *  colorDetect.h
 *  11/3/2025
 *
 *
 *  colorDetect.h will define the functionalities of the color detection
 */
#ifndef COLOR_DETECT_H
#define COLOR_DETECT_H

// Color class labels
enum ColorClass {
	COLOR_BLACK = 0,
	COLOR_BLUE  = 1,
	COLOR_YELLOW= 2,
	COLOR_RED   = 3
};

// Initializes the pins and captures ambient baseline from A5
void colorDetectSetup();

// Performs one measurement cycle and returns ColorClass
int detectColorClass(int delayMs = 50);

// Calculates Class 
int median5(int a, int b, int c, int d, int e);

// Calculates the closest color class based on the values
int calculateClass(int blue1, int yellow1, int red1);

#endif
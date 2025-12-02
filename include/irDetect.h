#ifndef IR_DETECT_H
#define IR_DETECT_H

// Initializes the pins and captures ambient baseline
void irDetectSetup(int delayMS);

// returns true if distance is past threshold (collision could happen)
bool detectDistance(int threshold);

#endifs
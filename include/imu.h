#ifndef IMU_H
#define IMU_H

#include <Arduino.h>

// Initialize the IMU. Returns true if successful.
bool imuSetup();

// Read gyroscope (deg/s) values (gx, gy, gz).
// Returns true if read was successful.
bool imuRead(float &gx, float &gy, float &gz);

#endif

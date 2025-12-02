#include "imu.h"
#include <Arduino.h>
#include <Arduino_LSM6DS3.h>

bool imuSetup() {
    if (!IMU.begin()) {
        Serial.println("LSM6DS3 not detected");
        return false;
    }
    Serial.println("LSM6DS3 initialized");
    return true;
}

bool imuRead(float &gx, float &gy, float &gz) {

    if (!IMU.gyroscopeAvailable()) {
        return false;
    }

    IMU.readGyroscope(gx, gy, gz);
    return true;
}

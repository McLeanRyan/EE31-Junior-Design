#include "imu.h"
#include <Arduino.h>
#include <Arduino_LSM6DS3.h>

static float headingZ = 0;
static float biasZ = 0;
static unsigned long lastTime = 0;

bool imuSetup() {
    if (!IMU.begin()) {
        Serial.println("LSM6DS3 not detected");
        return false;
    }
    Serial.println("LSM6DS3 initialized");
    float sum = 0;
    int samples = 500;

    for (int i = 0; i < samples; i++) {
        float gx, gy, gz;
        while (!IMU.gyroscopeAvailable());
        IMU.readGyroscope(gx, gy, gz);
        sum += gz;
        delay(5);
    }
    
    biasZ = sum / samples;
    lastTime = millis();

    return true;
}

bool imuRead(float &gx, float &gy, float &gz) {

    if (!IMU.gyroscopeAvailable()) {
        return false;
    }

    IMU.readGyroscope(gx, gy, gz);
    return true;
}

void imuUpdateHeading() {
    float gx, gy, gz;
    if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(gx, gy, gz);
        
        unsigned long now = millis();
        float dt = (now - lastTime) / 1000.0;
        lastTime = now;
        
        headingZ += (gz - biasZ) * dt;
    }
}

float imuGetHeading() {
    return headingZ;
}

void imuZeroHeading() {
    headingZ = 0;
    lastTime = millis();
}
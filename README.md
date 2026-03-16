# EE31 Design Project: Navigation & Multi-Bot Coordination

An integrated hardware and software system for autonomous mobile robots! This project implements a robust state-machine architecture to handle environmental perception (color and distance), precise navigation (IMU-based PID control), and real-time coordination via WebSocket servers.

## Key Features

* **PID-Controlled Navigation**: Utilizes the LSM6DS3 IMU to perform drift-corrected driving and precise axial turns using a PID control loop.
* **Intelligent Perception**: 
    * **Color Classification**: A custom training-based algorithm identifying Red, Blue, Yellow, and Black surfaces using an analog sensor and active LED illumination.
    * **Obstacle Detection**: IR-based proximity sensing with ambient light calibration for reliable wall detection.
* **Multi-Robot Synchronization**: Real-time communication via WebSockets to coordinate tasks between multiple robots (e.g., waiting for a partner to reach a specific state).
* **State-Machine Architecture**: A modular approach to autonomous routines, allowing for complex behaviors like lane following, wall-pivoting, and targeted color searching.

## System Architecture

The software is modularized into specialized drivers and high-level behavioral controllers:

* **Motor Control**: Low-level PWM mapping and high-level logic for differential drive maneuvers.
* **Sensor Fusion**: Integration of IMU heading data and IR distance readings to maintain spatial awareness.
* **Communication Layer**: A robust WebSocket client that parses server commands to facilitate "Self" vs "Partner" state updates.
* **Autonomous Routines**: `jointDemo.cpp` and `soloDemo.cpp` implement the high-level logic for specific project guidelines.

## Technical Details

* **Language**: C++
* **Framework**: Arduino (specifically targeting WiFiNINA compatible boards)
* **Communication**: WebSockets (TCP/IP)
* **Sensors**: LSM6DS3 (IMU), IR Proximity, Analog Color Sensing

## 🔧 Setup & Installation

1.  **Hardware**: Ensure the L298N motor driver, IMU, and sensors are wired according to the pin definitions in `motorcontrol.cpp` and `colorDetect.cpp`.
2.  **Configuration**: Create a `secrets.h` file containing:
    ```cpp
    #define WIFI_NAME "your_ssid"
    #define WIFI_PASSWORD "your_password"
    #define ADDRESS "server_ip"
    #define CLIENT_ID "your_bot_id"
    ```
3.  **Deployment**: Flash `main.cpp` using the Arduino IDE or VS Code with PlatformIO.

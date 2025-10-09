/*
 *  motorcontrol.h
 *  Hoang Mai
 *  10/02/2025
 *
 *
 *  Motor Class will represent the functionality that a motor could perform
 */

using namespace std;

class Motor {
public:
    Motor();

    void stop();
    void driveForward(int speed);
    void driveBackward(int speed);
    void pivotCW(int degree);
    void pivotCCW(int degree);
    void rightTurn(int turnRadius);
    void leftTurn(int turnRadius);


private:

};

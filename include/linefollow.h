#ifndef LINEFOLLOW_H
#define LINEFOLLOW_H

using namespace std;
#include "motorcontrol.h"

class LineFollow {
    
public:
    void followLeft(Motor& motor, int lineColor);
    void followRight(Motor& motor, int lineColor);
private:
};

#endif
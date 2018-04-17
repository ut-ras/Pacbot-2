#ifndef MOTOR_H
#define MOTOR_H
#include "enc.h"

class Motor {
    int m1;
    int m2;
    int pwm;
public:
    Enc enc;
    Motor(int m1pin, int m2pin, int pwmpin, int encA, int encB);
    ~Motor(); // Calls stop
    void set(double speed); // Speed is in range [-1,1]
    int getTicks();
    void zero();
    void stop();
};

#endif

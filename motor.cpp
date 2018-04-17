#include <math.h>
#include <pigpio.h>
#include "motor.h"

Motor::Motor(int m1pin, int m2pin, int pwmpin, int encA, int encB)
    : m1(m1pin), m2(m2pin), pwm(pwmpin), enc(encA, encB) {
    gpioSetMode(m1, PI_OUTPUT);
    gpioSetMode(m2, PI_OUTPUT);
    gpioSetMode(pwm, PI_OUTPUT);
    gpioSetPWMfrequency(pwm, 440);   // lower freq gives lower min speed
    gpioSetPWMrange(pwm, 1000);      // not sure why we'd want a small range
}

int Motor::getTicks(){
    return enc.pos;
}

void Motor::zero(){
    enc.pos = 0;
}

double DEADZONE = .05;

void Motor::set(double speed) {
    if(speed > DEADZONE) {
        gpioWrite(m1, 1);
        gpioWrite(m2, 0);
    } else if(speed < -DEADZONE) {
        gpioWrite(m1, 0);
        gpioWrite(m2, 1);
    } else {
        gpioWrite(m1, 0);
        gpioWrite(m2, 0);
    }
    gpioPWM(pwm, fabs(speed) * gpioGetPWMrange(pwm));
}

void Motor::stop() {
    set(0);
    gpioWrite(m1, 0);
    gpioWrite(m2, 0);
}

Motor::~Motor() {
    stop();
}

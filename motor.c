#include <math.h>
#include <stdlib.h>
#include <pigpio.h>
#include "motor.h"
#include "enc.h"

Motor* createMotor(int m1, int m2, int pwm, int encA, int encB) {
    Motor* m = malloc(sizeof(Motor));
    m->m1 = m1;
    m->m2 = m2;
    m->pwm = pwm;
    m->enc = createEnc(encA, encB);
    gpioSetMode(m1, PI_OUTPUT);
    gpioSetMode(m2, PI_OUTPUT);
    gpioSetMode(pwm, PI_OUTPUT);
    gpioSetPWMfrequency(pwm, 100);   // lower freq gives lower min speed
    gpioSetPWMrange(pwm, 1000);      // not sure why we'd want a small range
    return m;
}

void setMotor(Motor* m, float speed) {
    if(!m) {
        return;
    }
    if(speed > 0) {
        gpioWrite(m->m1, 1);
        gpioWrite(m->m2, 0);
    } else if(speed < 0) {
        gpioWrite(m->m1, 0);
        gpioWrite(m->m2, 1);
    }
    gpioPWM(m->pwm, fabs(speed) * gpioGetPWMrange(m->pwm));
}

void stopMotor(Motor* m) {
    if(!m) {
        return;
    }
    setMotor(m, 0);
    gpioWrite(m->m1, 0);
    gpioWrite(m->m2, 0);
}

void destroyMotor(Motor* m) {
    if(m) {
        stopMotor(m);
        destroyEnc(m->enc);
        free(m);
    }
}

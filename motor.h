#ifndef MOTOR_H
#define MOTOR_H

struct Motor {
    int m1;
    int m2;
    int pwm;
    struct Enc* enc;
};

typedef struct Motor Motor;

Motor* createMotor(int m1, int m2, int pwm, int encA, int encB);

// Speed is in range [-1,1]
void setMotor(Motor* m, float speed);

void destroyMotor(Motor* Motor);

#endif

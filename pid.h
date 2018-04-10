#ifndef PID_H
#define PID_H

struct PID {
    float goal;
    float prev;
    float sum;
    float kP;
    float kI;
    float kD;
};

typedef struct PID PID;

PID createPID(float p, float i, float d);

void setPID(PID* controller, float set);

float updatePID(PID* controller, float current, float dt);

#endif

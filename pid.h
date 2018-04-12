#ifndef PID_H
#define PID_H
#include <stdio.h>

struct PID {
    float goal;
    float prev;
    float sum;
    float kP;
    float kI;
    float kD;
    char* logName;
    FILE* logFile;
};

typedef struct PID PID;

// Pass NULL as logName to not log
PID* createPID(float p, float i, float d, char* logName);

void setPID(PID* controller, float set);

float updatePID(PID* controller, float current, float dt);

#endif

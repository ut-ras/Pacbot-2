#ifndef PID_H
#define PID_H
#include <stdio.h>

struct PID {
    double goal;
    double prev;
    double sum;
    double kP;
    double kI;
    double kD;
    char* logName;
    FILE* logFile;
};

typedef struct PID PID;

// Pass NULL as logName to not log
PID* createPID(double p, double i, double d, char* logName);

void setPID(PID* controller, double set);

double updatePID(PID* controller, double current, double dt);

void destroyPID(PID*);

#endif

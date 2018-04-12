#include <stdlib.h>
#include "pid.h"

#define MAX_SPEED (.4)
#define MAX_SUM (1000)
#define MAX(a, b) (a>b?a:b)
#define MIN(a, b) (a<b?a:b)

PID* createPID(float p, float i, float d, char* logName) {
    PID* temp = malloc(sizeof(PID));
    temp->kP = p;
    temp->kI = i;
    temp->kD = d;
    temp->logName = logName;
    if(logName) {
        temp->logFile = fopen(logName, "w");
    }
    return temp;
}

void setPID(PID* controller, float set) {
    controller->goal = set;
    controller->sum = 0;
    controller->prev = 0;
    if(controller->logName) {
        fprintf(controller->logFile, "\nSet: %f P: %.5f I: %.5f D: %.5f\n", set,
                controller->kP, controller->kI, controller->kD);
    }
}

float updatePID(PID* controller, float current, float dt) {
    if(*controller->logName) {
        fprintf(controller->logFile, "%f %f\n", dt, current);
    }
    float err = controller->goal - current;
    float temp = controller->prev;
    controller->sum += err * dt;
    controller->sum = MIN(MAX(controller->sum, -MAX_SUM), MAX_SUM);
    controller->prev = err;
    // printf("%f\t\t%f\n", controller->kI * controller->sum, controller->kD * (current - temp) / dt);
    return controller->kP * err +
           controller->kI * controller->sum +
           controller->kD * (err - temp) / dt;
}

void destroyPID(PID* controller) {
    if(controller->logName) {
        fclose(controller->logFile);
    }
    free(controller);
}

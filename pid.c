#include <stdlib.h>
#include "pid.h"

#define MAX_SPEED (.4)
#define MAX_SUM (1000)
#define MAX(a, b) (a>b?a:b)
#define MIN(a, b) (a<b?a:b)

PID* createPID(double p, double i, double d, char* logName) {
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

void setPID(PID* controller, double set) {
    controller->goal = set;
    controller->sum = 0;
    controller->prev = 0;
    if(controller->logName) {
        fprintf(controller->logFile, "\nSet: %f P: %f I: %f D: %f\n", set,
                controller->kP, controller->kI, controller->kD);
    }
}

double updatePID(PID* controller, double current, double dt) {
    double err = controller->goal - current;
    double temp = controller->prev;
    double d = (err - temp) / dt;
    controller->sum += err * dt;
    controller->sum = MIN(MAX(controller->sum, -MAX_SUM), MAX_SUM);
    controller->prev = err;
    // printf("%f\t\t%f\n", controller->kI * controller->sum, controller->kD * (current - temp) / dt);
    double result = controller->kP * err +
                    controller->kI * controller->sum +
                    controller->kD * d;
    if(*controller->logName) {
        fprintf(controller->logFile, "%f %f %f %f %f\n", dt, controller->kP * err,
                controller->kD * d, controller->kI * controller->sum, result);
    }
    return result;
}

void destroyPID(PID* controller) {
    if(controller->logName) {
        fclose(controller->logFile);
    }
    free(controller);
}

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <pigpio.h>
#include "base.h"
#include "motor.h"
#include "enc.h"
#include "pid.h"

double test_speed = .4;

PID* controller;
Motor* m;
double dt = .01; // fastest possible callbacks with pigpio
double maxSpeed = .6;

Motor* motors[4];
PID* pids[4];


void cleanup() {
    signal(SIGINT, SIG_IGN);
    stop();
    terminateBase();
    gpioTerminate();
    printf("\nGPIO Terminated\n");
    exit(0);
}

void testIndividualMotor(Motor* m) {
    setMotor(m, test_speed);
    time_sleep(.5);
    setMotor(m, -test_speed);
    time_sleep(.5);
    stopMotor(m);
    time_sleep(.5);
}

void testBase() {
    signal(SIGINT, cleanup);
    Motor* top = createMotor(15, 14, 18, 27, 17);
    testIndividualMotor(top);
    Motor* left = createMotor(10, 9, 11, 25, 8);
    testIndividualMotor(left);
    Motor* bottom = createMotor(16, 20, 21, 19, 26);
    testIndividualMotor(bottom);
    Motor* right = createMotor(5, 6, 13, 7, 1);
    testIndividualMotor(right);
    initBase(top, bottom, left, right);
    printEncoders();
    for(int i = 0; i < 4; ++i) {
        spin(test_speed);
        time_sleep(1);
        stop();
        time_sleep(.5);
        printEncoders();
        test_speed = -test_speed;
    }
    cleanup();
}

void baseCB() {
    for(int i = 0; i < 4; ++i) {
        double next = updatePID(pids[i], motors[i]->enc->pos, dt);
        if(next > maxSpeed) {
            next = maxSpeed;
        } else if(next < -maxSpeed) {
            next = -maxSpeed;
        }
        // printf("Enc: %ld\t\tMotor: %f\n", m->enc->pos, next);
        setMotor(motors[i], next);
    }
}

int main() {
    if (gpioInitialise() < 0) {
        return 1;
    }
    Motor* top = createMotor(15, 14, 18, 17, 27);
    Motor* left = createMotor(10, 9, 11, 8, 25);
    Motor* bottom = createMotor(16, 20, 21, 26, 19);
    Motor* right = createMotor(5, 6, 13, 1, 7);
    motors[0] = top;
    motors[1] = bottom;
    motors[2] = left;
    motors[3] = right;
    char* logs[] = {"top.txt", "bot.txt", "left.txt", "right.txt"};
    for(int i = 0; i < 4; ++i) {
        pids[i] = createPID(.01, .0003, .0005, logs[i]);
        setPID(pids[i], 0);
    }

    gpioSetTimerFunc(0, dt * 1000, baseCB);
    int setPoint = 0;
    while(setPoint != -1) {
        setPID(pids[0], setPoint);
        setPID(pids[1], -setPoint);
        printf("Input next set point (-1 to exit): ");
        scanf("%d", &setPoint);
    }
    for(int i=0;i<4;++i){
        destroyMotor(motors[i]);
        destroyPID(pids[i]);
    }
}

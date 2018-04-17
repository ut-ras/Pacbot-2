#include <math.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <pigpio.h>
#include "base.h"
#include "motor.h"
#include "enc.h"
#include "pid.h"

double dt = .05; // fastest possible callbacks with pigpio
double maxSpeed = .6;
double test_speed = .4;

Motor* motors[4];
PID* pids[4];


void cleanup() {
    signal(SIGINT, SIG_IGN);
    terminateBase();
    gpioTerminate();
    printf("\nGPIO Terminated\n");
    exit(0);
}

void testIndividualMotor() {
    Motor* m = createMotor(15, 14, 18, 17, 27);
    double duty = 0;
    double freq = 500;
    while(duty != -1) {
        gpioSetPWMfrequency(18, freq);   // lower freq gives lower min speed
        setMotor(m, duty);
        scanf("%lf %lf", &duty, &freq);
    }
    destroyMotor(m);
}

int mainBase() {
    signal(SIGINT, cleanup);
    Motor* top = createMotor(15, 14, 18, 17, 27);
    Motor* left = createMotor(10, 9, 11, 8, 25);
    Motor* bottom = createMotor(16, 20, 21, 26, 19);
    Motor* right = createMotor(5, 6, 13, 1, 7);
    testIndividualMotor(top);
    testIndividualMotor(left);
    testIndividualMotor(bottom);
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
    return 0;
}

void baseCB() {
    double next[4];
    for(int i = 0; i < 4; ++i) {
        next[i] = updatePID(pids[i], motors[i]->enc->pos, dt);
        if(next[i] > maxSpeed) {
            next[i] = maxSpeed;
        } else if(next[i] < -maxSpeed) {
            next[i] = -maxSpeed;
        }
    }
    driveDirect(next[0], next[1], next[2], next[3]);
}

int main() {
    if (gpioInitialise() < 0) {
        return 1;
    }
    // testIndividualMotor();
    // return 0;
    signal(SIGINT, cleanup);
    motors[0] = createMotor(15, 14, 18, 17, 27);
    motors[1] = createMotor(16, 20, 21, 26, 19);
    motors[2] = createMotor(10, 9, 11, 8, 25);
    motors[3] = createMotor(5, 6, 13, 1, 7);
    initBase(motors[0], motors[1], motors[2], motors[3]);

    char* logs[] = {"top.txt", "bot.txt", "left.txt", "right.txt"};
    for(int i = 0; i < 4; ++i) {
        pids[i] = createPID(.001, .0000, .0000, logs[i]);
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
    // while(1){
    //     printf("%.5f %.5f\n", pids[2]->prev, pids[2]->sum);
    // }
    for(int i = 0; i < 4; ++i) {
        destroyPID(pids[i]);
    }
    cleanup();
    return 0;
}

double deltaMotor = .01;
double maxMotor = .5;
double minMotor = .1;
int deadzone = 10;
double currentSpeed = 0;
int setPoint = 0;
int startPoint = 0;
int done = 1;
double dist = 0;
void trapezoidCallback() {
    if(done) {
        return;
    }
    int ticks = motors[0]->enc->pos;
    // printf("%.3lf\t%d\n", currentSpeed, ticks);
    if(fabs(dist) - abs(ticks) < deadzone) {
        done = 1;
        printf("we did it: %d\n", motors[0]->enc->pos);
        currentSpeed = 0;
        motors[0]->enc->pos = 0;
        stopMotor(motors[0]);
        stopMotor(motors[1]);
        return;
    }
    if(abs(ticks) > dist / 2) {
        // slow
        currentSpeed -= deltaMotor;
    } else {
        // fast
        currentSpeed += deltaMotor;
    }
    if(fabs(currentSpeed) > maxSpeed) {
        // flat
        setMotor(motors[0], maxSpeed);
        setMotor(motors[1], -maxSpeed);
    } else {
        setMotor(motors[0], currentSpeed);
        setMotor(motors[1], -currentSpeed);
    }
}

int mainTrap() {
    if (gpioInitialise() < 0) {
        return 1;
    }
    signal(SIGINT, cleanup);
    motors[0] = createMotor(15, 14, 18, 17, 27);
    motors[1] = createMotor(16, 20, 21, 26, 19);
    motors[2] = createMotor(10, 9, 11, 8, 25);
    motors[3] = createMotor(5, 6, 13, 1, 7);
    gpioSetTimerFunc(0, dt * 1000, trapezoidCallback);
    while(setPoint != -1) {
        printf("Input next set point (-1 to exit): ");
        scanf("%d", &setPoint);
        done = 0;
        dist = abs(setPoint - startPoint);
        // printf("%d\n", motors[0]->enc->pos);
        // time_sleep(.5);
    }
    cleanup();
}


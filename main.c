#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <pigpio.h>
#include "base.h"
#include "motor.h"
#include "enc.h"
#include "pid.h"

int dur = 4;
float test_speed = .5;

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
    Motor* top = createMotor(11, 25, 18, 7, 8);
    testIndividualMotor(top);
    Motor* bottom = createMotor(17, 27, 22, 23, 24);
    testIndividualMotor(bottom);
    // Motor* left = createMotor(10, 9, 11, 19, 26); // when initialized, prevents top from turning for some reason. check wiring maybe?
    // testIndividualMotor(left);
    // Motor* right = createMotor(5, 6, 13, 12, 16);
    // testIndividualMotor(right);
    initBase(top, bottom, NULL, NULL);
    printEncoders();
    for(int i = 0; i < dur; ++i) {
        spin(test_speed);
        time_sleep(1);
        stop();
        time_sleep(.5);
        printEncoders();
        test_speed = -test_speed;
    }
    cleanup();
}

PID controller;
Motor* m;
int elapsed = 50;
float maxSpeed = .6;

void pidCB() {
    float next = updatePID(&controller, m->enc->pos, ((float)elapsed) / 1000.f);
    if(next > maxSpeed) {
        next = maxSpeed;
    } else if(next < -maxSpeed) {
        next = -maxSpeed;
    }
    // printf("Enc: %ld\t\tMotor: %f\n", m->enc->pos, next);
    setMotor(m, next);
}

int main(int argc, char* argv[]) {
    if (gpioInitialise() < 0) {
        return 1;
    }
    controller = createPID(.01, .0001, .0005);
    m = createMotor(17, 27, 22, 24, 23);
    int setPoint = 0;

    testIndividualMotor(m);
    m->enc->pos = 0;
    setPID(&controller, setPoint);
    gpioSetTimerFunc(0, elapsed, pidCB);
    while(setPoint >= 0) {
        setPID(&controller, setPoint);
        printf("Input next set point (negative to exit): ");
        scanf("%d", &setPoint);
    }
    stopMotor(m);
    destroyMotor(m);
    return 0;
}


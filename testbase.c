#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <pigpio.h>
#include "base.h"
#include "motor.h"
#include "enc.h"

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

void testIndividualMotor(Motor* m){
    setMotor(m, test_speed);
    time_sleep(1);
    setMotor(m, -test_speed);
    time_sleep(1);
    stopMotor(m);
}

int main(int argc, char* argv[]) {
    signal(SIGINT, cleanup);
    if (gpioInitialise() < 0) {
        return 1;
    }
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


#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <pigpio.h>
#include "base.h"
#include "motor.h"
#include "enc.h"

int dur = 5;
float test_speed = .5;

void sigintHandler(int sig_num) {
    printf("\nTERMINATING\n");
    fflush(stdout);
    terminateBase();
    gpioTerminate();
    exit(1);
}

int main(int argc, char* argv[]) {
    signal(SIGINT, sigintHandler);
    if (gpioInitialise() < 0) {
        return 1;
    }
    initBase(createMotor(11, 25, 18, 7, 8), createMotor(17, 27, 22, 23, 24),
             createMotor(10, 9, 11, 19, 26), createMotor(5, 6, 13, 12, 16));
    printEncoders();
    spin(test_speed);
    for(int i = 0; i < dur; ++i) {
        time_sleep(1);
        printEncoders();
        test_speed = -test_speed;
        spin(test_speed);
    }
    time_sleep(1); // to let them slow down
    printEncoders();
    terminateBase();
    gpioTerminate();
    return 0;
}


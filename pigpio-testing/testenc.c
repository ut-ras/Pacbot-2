#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <pigpio.h>
#include "motor.h"
#include "enc.h"

// build with: -lpigpio -lrt and run with sudo

float test_speed = .7;
int dur = 5;
Motor* m[2];

void sigintHandler(int sig_num) {
    printf("\nTERMINATING\n");
    fflush(stdout);
    destroyMotor(m[0]);
    destroyMotor(m[1]);
    gpioTerminate();
    exit(1);
}

int main(int argc, char* argv[]) {
    signal(SIGINT, sigintHandler);
    if (gpioInitialise() < 0) {
        return 1;
    }
    m[0] = createMotor(11, 25, 18, 7, 8);
    m[1] = createMotor(17, 27, 22, 23, 24);
    printf("pos:%ld\t\t%ld\n", m[0]->enc->pos, m[1]->enc->pos);
    setMotor(m[0], test_speed);
    setMotor(m[1], test_speed);
    for(int i = 0; i < dur; ++i) {
        time_sleep(1);
        printf("pos:%ld\t\t%ld\n", m[0]->enc->pos, m[1]->enc->pos);
    }
    setMotor(m[0], 0);
    setMotor(m[1], 0);
    time_sleep(1); // to let them slow down
    printf("final pos:%ld\t\t%ld\n", m[0]->enc->pos, m[1]->enc->pos);
    destroyMotor(m[0]);
    destroyMotor(m[1]);
    gpioTerminate();
    return 0;
}


#include <pigpio.h>
#include <iostream>
#include <csignal>
#include <cstdlib>
#include "../inc/motor.h"

using namespace std;

void cleanup(int signum) {
    signal(SIGINT, SIG_IGN);
    gpioTerminate();
    cout << "GPIO Terminated" << endl;
    exit(0);
}

int main() {
    if (gpioInitialise() < 0) {
        return 1;
    }
    signal(SIGINT, cleanup);
    L298N m(15, 14, 18, 17, 27);
    // motors[1] = createMotor(16, 20, 21, 26, 19);
    // motors[2] = createMotor(10, 9, 11, 8, 25);
    // motors[3] = createMotor(5, 6, 13, 1, 7);
    m.set(.25);
    for(int i = 0; i < 5; ++i) {
        time_sleep(.5);
        cout << m.getTicks() << endl;
    }
    cleanup(SIGINT);
}

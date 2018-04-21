#include <pigpio.h>
#include <iostream>
#include <csignal>
#include <cstdlib>
#include <vector>
#include "../inc/motor.h"
#include "../inc/pid.h"

using namespace std;

// Positive is forward/clockwise in all contexts
// Coordinates increase in down and right directions
// Motor numbering is 0..4 = top, bottom, left, right

int loopSpeed = 50; // in ms (10 is fastest pigpio will allow)
double maxSpeed = .6; // percentage

enum axis {horizontal, vertical};
vector<PID*> linearControl(4);
PID* rotationControl;
PID* alignmentControl;
vector<Motor*> motors(4);

// Run all control loops in series
void controlCB() {
    double gains[4];
    for(int i = 0; i < 4; ++i) {
        gains[i] = linearControl[i]->update(motors[i]->getTicks(),
                                            double(loopSpeed) / 1000.);
    }
    // double angularCorrection = rotationControl->update(imu input here);
    // double alignmentCorrection = alignmentControl->update(vlx input here);
    for(int i = 0; i < 4; ++i) {
        // optional velocity pid here to sacrifice positional precision for
        // more easily regulated acceleration
        motors[i]->set(max(min(gains[i], maxSpeed), -maxSpeed));
    }
}

void moveStraight(int dist, axis dir) {
    for(int i = 0; i < 2; ++i) {
        int m = i + (dir == vertical);
        motors[m]->zero();
        linearControl[m]->set(dist * (i - 1));
    }
}

void moveDir(int dist, int dir) {
    switch(dir) {
    case 0:
        moveStraight(-dist, vertical);
        break;
    case 1:
        moveStraight(dist, vertical);
        break;
    case 2:
        moveStraight(-dist, horizontal);
        break;
    case 3:
        moveStraight(dist, horizontal);
        break;
    default:
        break;
    }
}

void enableCB(int millis) {
    gpioSetTimerFunc(0, millis, controlCB);
}

void disableCB() {
    gpioSetTimerFunc(0, 0, 0);
}

void cleanup(int signum) {
    signal(SIGINT, SIG_IGN);
    disableCB();
    gpioTerminate();
    cout << "GPIO Terminated" << endl;
    exit(0);
}

int main() {
    if (gpioInitialise() < 0) {
        return 1;
    }
    signal(SIGINT, cleanup);
    motors[0] = new L298N(15, 14, 18, 17, 27);
    motors[1] = new L298N(16, 20, 21, 26, 19);
    motors[2] = new L298N(10, 9, 11, 8, 25);
    motors[3] = new L298N(5, 6, 13, 1, 7);
    for(int i=0;i<4;++i){
        linearControl[i] = new PID(.008, .0005, .0003);
    }
    // rotationControl = new PID(p, i, d);
    // alignmentControl = new PID(p, i, d);
    enableCB(loopSpeed);
    // testing pid with a single motor
    // int setPoint = 0;
    // while(setPoint != -1){
    //     cout << "input setpoint: ";
    //     cin >> setPoint;
    //     linearControl[0]->set(setPoint);
    // }

    int testDist = 0;
    int testDir = 0;
    while(testDist != -1 && testDir !=-1) {
        cout << "Input a distance in encoder ticks: ";
        cin >> testDist;
        cout << "Input a direction (0..4 = up down left right): ";
        cin >> testDir;
        moveDir(testDist, testDir);
        // check for error conditions
        // time_sleep(.5);
    }
    cleanup(SIGINT);
}

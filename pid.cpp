#include <stdlib.h>
#include "pid.h"

#define MAX_SPEED (.4)
#define MAX_SUM (1000)
#define MAX(a, b) (a>b?a:b)
#define MIN(a, b) (a<b?a:b)

PID::PID(double p, double i, double d, string logName) : kP(p), kI(i), kD(d) {
    logFile.open(logname, std::fstream::app); // change to "out" to verwrite
}

void PID::set(double set) {
    goal = set;
    sum = 0;
    prev = 0;
    if(logFile.is_open()) {
        logfile << std::endl << "Set: " << set << " P: " << kP << " I: "
                << kI << " D: " << kD << std::endl;
    }
}

double PID::update(double current, double dt) {
    double err = goal - current;
    double d = (err - prev) / dt;
    sum += err * dt;
    // sum = MIN(MAX(controller->sum, -MAX_SUM), MAX_SUM);
    prev = err;
    double result = kP * err + kI * sum + kD * d;
    if(logFile.is_open()) {
        logFile << dt << " " << kP* err << " " << kI* sum << " " << kD* d
                << " " << result << std::endl;
    }
    return result;
}

PID::~PID destroyPID() {
    if(logFile.is_open()) {
        logFile.close();
    }
}

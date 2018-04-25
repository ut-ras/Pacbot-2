#include <Python.h>
#include <pigpio.h>
#include <iostream>
#include <vector>
#include "../inc/motor.h"
#include "../inc/pid.h"

using namespace std;

// Positive is forward/clockwise in all contexts
// Coordinates increase in down and right directions
// Motor numbering is 0..4 = top, bottom, left, right

const int ticksPerUnit = 400;
const double maxSpeed = .60; // percentage

enum axis {horizontal, vertical};
vector<PID*> linearControl(4);
PID* rotationControl;
PID* alignmentControl;
vector<Motor*> motors(4);

// Run all control loops in series
double gains[4];
void controlUpdate(double elapsed) {
    for(int i = 0; i < 4; ++i) {
        gains[i] = linearControl[i]->update(motors[i]->getTicks(), elapsed);
    }
    // double angularCorrection = rotationControl->update(mpu input here);
    // double alignmentCorrection = alignmentControl->update(vlx input here);
    for(int i = 0; i < 4; ++i) {
        // optional velocity pid here to sacrifice positional precision for
        // more easily regulated acceleration
        motors[i]->set(max(min(gains[i], maxSpeed), -maxSpeed));
    }
}

const int steady = 5;
const int threshold = 10;
void moveStraight(int dist, axis dir) {
    int s, us;
    gpioTime(0, &s, &us);
    int ready = 0;
    double currentTime, lastTime = (double(s) * 1000 + double(us / 1000)) / 1000;
    for(int i = 0; i < 2; ++i) {
        int m = i + (dir == vertical);
        motors[m]->zero();
        linearControl[m]->set(dist * (i - 1));
    }
    //block until you're close enough to the set point for long enough
    while(ready < steady) {
        gpioTime(0, &s, &us);
        currentTime = (double(s) * 1000 + double(us / 1000)) / 1000;
        controlUpdate(currentTime - lastTime);
        time_sleep(.01); // run pid around 100 times per second
        ++ready;
        for(int i = 0; i < 4; ++i) {
            if(linearControl[i]->getErr() > threshold) {
                // cout << "holdup: " << linearControl[i]->getErr() << endl;
                ready = 0;
                break;
            }
        }
        lastTime = currentTime;
    }
}

static PyObject* hardStop(PyObject* self, PyObject* args) {
    for(int i = 0; i < 4; ++i) {
        motors[i]->stop();
    }
    return Py_BuildValue("i", 1);
}

static PyObject* moveDir(PyObject* self, PyObject* args) {
    int dist, dir;
    PyArg_ParseTuple(args, "ii", &dist, &dir);
    cout << "moving " << dist << " units in direction: " << dir << endl;
    dist *= ticksPerUnit;
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
        cout << "Invalid direction, stopping" << endl;
        hardStop(self, args);
        break;
    }
    return Py_BuildValue("i", 1);
}

static PyObject* initRobot(PyObject* self, PyObject* args) {
    if (gpioInitialise() < 0) {
        return Py_BuildValue("i", 0);
    }
    motors[0] = new L298N(15, 14, 18, 17, 27);
    motors[1] = new L298N(16, 20, 21, 26, 19);
    motors[2] = new L298N(10, 9, 11, 8, 25);
    motors[3] = new L298N(5, 6, 13, 1, 7);
    for(int i = 0; i < 4; ++i) {
        linearControl[i] = new PID(.008, .0005, .0003);
    }
    // rotationControl = new PID(p, i, d);
    // alignmentControl = new PID(p, i, d);
    return Py_BuildValue("i", 1);
}

static PyMethodDef halMethods[] = {
    {"initRobot", initRobot, METH_VARARGS, NULL},
    {"hardStop", hardStop, METH_VARARGS, NULL},
    {"moveDir", moveDir, METH_VARARGS, NULL}
};

static struct PyModuleDef halmodule = {
    PyModuleDef_HEAD_INIT,
    "hal",
    NULL,
    -1,
    halMethods
};

PyMODINIT_FUNC PyInit_hal(void) {
    return PyModule_Create(&halmodule);
}

int main(int argc, char* argv[]) {
    wchar_t* program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL) {
        fprintf(stderr, "RIP");
        exit(1);
    }
    PyImport_AppendInittab("hal", PyInit_hal);
    Py_SetProgramName(program);
    Py_Initialize();
    PyImport_ImportModule("hal");
    PyMem_RawFree(program);
    return 0;
}



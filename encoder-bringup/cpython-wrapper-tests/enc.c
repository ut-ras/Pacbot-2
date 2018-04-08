#include <Python.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <wiringPi.h>

typedef struct Channel {
    long pos;
    uint8_t state;
    uint8_t pinA;
    uint8_t pinB;
    uint8_t enable;
} Channel;

Channel enc[4];

// Inspired by Paul Stoffregen's excellent Arduino library Encoder:
// https://github.com/PaulStoffregen/Encoder
void pin_isr(int ch) {
    uint8_t p1val = digitalRead(enc[ch].pinA);
    uint8_t p2val = digitalRead(enc[ch].pinB);
    uint8_t s = enc[ch].state & 3;
    if (p1val) {
        s |= 4;
    }
    if (p2val) {
        s |= 8;
    }
    enc[ch].state = (s >> 2);

    switch (s) {
    case 1:
    case 7:
    case 8:
    case 14:
        enc[ch].pos++;
        return;
    case 2:
    case 4:
    case 11:
    case 13:
        enc[ch].pos--;
        return;
    case 3:
    case 12:
        enc[ch].pos += 2;
        return;
    case 6:
    case 9:
        enc[ch].pos -= 2;
        return;
    }
}

void isr0(void) {
    pin_isr(0);
}
void isr1(void) {
    pin_isr(1);
}
void isr2(void) {
    pin_isr(2);
}
void isr3(void) {
    pin_isr(3);
}
void (*isr_list[4])(void) = {isr0, isr1, isr2, isr3};

// real signature: (int channel, int pinA, int pinB)
static PyObject* initChannel(PyObject* self, PyObject* args) {
    int channel, pinA, pinB;
    if (!PyArg_ParseTuple(args, "iii", &channel, &pinA, &pinB)) {
        return NULL;
    }

    enc[channel].pinA = pinA;
    enc[channel].pinB = pinB;
    enc[channel].enable = 1;
    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
    pullUpDnControl(pinA, PUD_UP);
    pullUpDnControl(pinB, PUD_UP);

    if (wiringPiISR (pinA, INT_EDGE_BOTH, isr_list[channel]) < 0) {
        return Py_BuildValue("i", -1);
    }
    if (wiringPiISR (pinB, INT_EDGE_BOTH, isr_list[channel]) < 0) {
        return Py_BuildValue("i", -1);
    }
    return Py_BuildValue("i", 0);
}

// real signature: (int channel)
static PyObject* getTicks(PyObject* self, PyObject* args) {
    int ch;
    if (!PyArg_ParseTuple(args, "i", &ch)) {
        return NULL;
    }
    return Py_BuildValue("i", enc[ch].pos * enc[ch].enable);
}

static PyMethodDef encMethods[] = {
    {"initChannel", initChannel, METH_VARARGS, NULL},
    {"getTicks", getTicks, METH_VARARGS, NULL}
};

static struct PyModuleDef encmodule = {
    PyModuleDef_HEAD_INIT, "enc", NULL, -1, encMethods
};

PyMODINIT_FUNC PyInit_enc(void) {
    for(int i = 0; i < 4; ++i) {
        enc[i].enable = 0;
        enc[i].state = 0;
        enc[i].pos = 0;
    }
    if (wiringPiSetup() < 0) {
        return Py_BuildValue("i", -1);
    }
    return PyModule_Create(&encmodule);
}

int main(int argc, char* argv[]) {
    wchar_t* program = Py_DecodeLocale(argv[0], NULL);

    if (program == NULL) {
        fprintf(stderr, "RIP");
        exit(1);
    }

    PyImport_AppendInittab("enc", PyInit_enc);
    Py_SetProgramName(program);
    Py_Initialize();
    PyImport_ImportModule("enc");
    PyMem_RawFree(program);
    return 0;
}


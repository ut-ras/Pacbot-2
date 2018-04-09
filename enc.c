#include <stdlib.h>
#include <pigpio.h>
#include "enc.h"

static void _cb(int gpio, int level, uint32_t tick, void* user) {
    Enc* enc = user;
    if (gpio == enc->encA) {
        enc->levA = level;
    } else {
        enc->levB = level;
    }
    if (gpio != enc->lastGpio) { // Debounce
        enc->lastGpio = gpio;
        if ((gpio == enc->encA) && (level == 1)) {
            if (enc->levB) {
                ++enc->pos;
            }
        } else if ((gpio == enc->encB) && (level == 1)) {
            if (enc->levA) {
                --enc->pos;
            }
        }
    }
}

Enc* createEnc(int encA, int encB) {
    Enc* enc = malloc(sizeof(Enc));
    enc->encA = encA;
    enc->encB = encB;
    enc->levA = 0;
    enc->levB = 0;
    enc->pos = 0;
    enc->pos = 0;
    enc->lastGpio = -1;
    gpioSetMode(encA, PI_INPUT);
    gpioSetMode(encB, PI_INPUT);
    gpioSetPullUpDown(encA, PI_PUD_UP);
    gpioSetPullUpDown(encB, PI_PUD_UP);
    gpioSetAlertFuncEx(encA, _cb, enc);
    gpioSetAlertFuncEx(encB, _cb, enc);
    return enc;
}

void destroyEnc(Enc* enc) {
    if (enc) {
        gpioSetAlertFunc(enc->encA, 0);
        gpioSetAlertFunc(enc->encB, 0);
        free(enc);
    }
}


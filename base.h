#ifndef BASE_H
#define BASE_H
#include "motor.h"

void initBase(Motor* t, Motor* b, Motor* l, Motor* r);

void printEncoders();

void drive(float x, float y);

void spin(float speed);

void stop();

void terminateBase();

#endif

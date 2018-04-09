#include <stdio.h>
#include "base.h"
#include "enc.h"

Motor* top;
Motor* bottom;
Motor* left;
Motor* right;

void initBase(Motor* t, Motor* b, Motor* l, Motor* r) {
    top = t;
    bottom = b;
    left = l;
    right = r;
}

void printEncoders() {
    printf("Top: %06ld\tBottom: %06ld\tLeft: %06ld\tRight: %06ld\n",
           top->enc->pos, bottom->enc->pos, left->enc->pos, right->enc->pos);
}

void drive(float x, float y) {
    setMotor(top, x);
    setMotor(bottom, -x);
    setMotor(left, y);
    setMotor(right, -y);
}

void spin(float speed) {
    setMotor(top, speed);
    setMotor(bottom, speed);
    setMotor(left, speed);
    setMotor(right, speed);
}

void terminateBase() {
    destroyMotor(top);
    destroyMotor(bottom);
    destroyMotor(left);
    destroyMotor(right);
}


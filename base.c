#include <stdio.h>
#include "base.h"
#include "enc.h"

Motor* top = NULL;
Motor* bottom = NULL;
Motor* left = NULL;
Motor* right = NULL;

void initBase(Motor* t, Motor* b, Motor* l, Motor* r) {
    top = t;
    bottom = b;
    left = l;
    right = r;
}

void printEncoders() {
    if(top) {
        printf("Top: %06ld\t", top->enc->pos);
    }
    if(bottom) {
        printf("Bottom: %06ld\t", bottom->enc->pos);
    }
    if(left) {
        printf("Left: %06ld\t", left->enc->pos);
    }
    if(right) {
        printf("Right: %06ld\t", right->enc->pos);
    }
    printf("\n");
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

void stop() {
    stopMotor(top);
    stopMotor(bottom);
    stopMotor(left);
    stopMotor(right);
}

void terminateBase() {
    destroyMotor(top);
    destroyMotor(bottom);
    destroyMotor(left);
    destroyMotor(right);
}


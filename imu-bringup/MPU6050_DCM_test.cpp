//
// Created by jspspike on 4/12/18.
//
/*
To compile on a Raspberry Pi (1 or 2)
1. install the bcm2835 library, see http://www.airspayce.com/mikem/bcm2835/index.html
2. enable i2c on your RPi , see https://learn.adafruit.com/adafruits-raspberry-pi-lesson-4-gpio-setup/configuring-i2c
3. connect your i2c devices
4. then from bash
        $ PATH_I2CDEVLIB=~/i2cdevlib/
        $ gcc -o MPU6050_DCM_test ${PATH_I2CDEVLIB}RaspberryPi_bcm2835/MPU6050/examples/MPU6050_DCM_test.cpp \
         -I ${PATH_I2CDEVLIB}RaspberryPi_bcm2835/I2Cdev ${PATH_I2CDEVLIB}RaspberryPi_bcm2835/I2Cdev/I2Cdev.cpp \
         -I ${PATH_I2CDEVLIB}/Arduino/MPU6050/ ${PATH_I2CDEVLIB}/Arduino/MPU6050/MPU6050.cpp -l bcm2835 -l m
$ sudo ./MPU6050_DCM_test

*/

#include <stdio.h>
#include <bcm2835.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include <pigpio.h>
//#include "../../I2Cdev/I2Cdev.h"
//#include "../../../MSP430/MPU6050/MPU6050z.h"
//#include "../../../Arduino/MPU6050/helper_3dmath.h"
#include <math.h>
#include <cstdint>

MPU6050 gyro;
uint16_t packetSize;

Quaternion q;
float euler[3];



void getGyroValues(int GPIO, int level, uint32_t tick) {

    uint16_t fifoCount = gyro.getFIFOCount();
    uint8_t fifoBuffer[64];
    uint8_t mpuIntStatus = gyro.getIntStatus();


    if ((mpuIntStatus & 0x10) || (fifoCount == 1024)) {
        gyro.resetFIFO();
        printf("FIFO Overflow\r");
    } else if (mpuIntStatus & 0x02) {
        while ((fifoCount = gyro.getFIFOCount()) < packetSize) {}

        gyro.getFIFOBytes(fifoBuffer, packetSize);

        fifoCount -= packetSize;

        gyro.dmpGetQuaternion(&q, fifoBuffer);
        gyro.dmpGetEuler(euler, &q);
    }
    else
        printf("MPUIntStatus incorrect\n");
}

int gyroInit() {
    gpioInitialise();
    gpioSetMode(22, PI_INPUT);
    gpioSetISRFunc(22, RISING_EDGE, 0, getGyroValues);

    printf("Starting DMP MPU6050 test\n");
    I2Cdev::initialize();

    if (gyro.testConnection())
        printf("Connected\n");
    else
        printf("Connection failed\nContinuing anyways\n");

    printf("Init Gyro\n");
    gyro.initialize();
    printf("Init DMP\n");
    int dmpStatus = gyro.dmpInitialize();



    gyro.setXGyroOffset(220);
    gyro.setYGyroOffset(76);
    gyro.setZGyroOffset(-85);
    gyro.setZAccelOffset(1788);

    if (dmpStatus == 0) {
        printf("Enabling DMP\n");
        gyro.setDMPEnabled(true);
    }
    else {
        printf("DMP Init failed\n");
        return 1;
    }

    packetSize = gyro.dmpGetFIFOPacketSize();

    return 0;
}

void printGyroValues() {
    printf("Euler: ");
    printf("X: %f         ", euler[0] * 180/M_PI);
    printf("Y: %f         ", euler[1] * 180/M_PI);
    printf("Z: %f\r", euler[2] * 180/M_PI);

}

int main(int argc, char **argv) {

    if (gyroInit())
        return 1;

    while (true) {
        printGyroValues();
    }

    return 0;
}
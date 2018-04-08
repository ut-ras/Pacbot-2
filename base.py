import RPIO
from motor import Motor
from pinout import *

class Base:

    def __init__(self):
        RPIO.PWM.setup()
        self.top = Motor(
            TOP_PIN_1,
            TOP_PIN_2,
            TOP_PIN_PWM,
            TOP_ENC_A,
            TOP_ENC_B,
            0)
        self.bottom = Motor(
            BOTTOM_PIN_1,
            BOTTOM_PIN_2,
            BOTTOM_ENC_A,
            BOTTOM_ENC_B,
            BOTTOM_PIN_PWM,
            1)
        self.left = Motor(
            LEFT_PIN_1,
            LEFT_PIN_2,
            LEFT_PIN_PWM,
            LEFT_ENC_A,
            LEFT_ENC_B,
            2)
        self.right = Motor(
            RIGHT_PIN_1,
            RIGHT_PIN_2,
            RIGHT_PIN_PWM,
            RIGHT_ENC_A,
            RIGHT_ENC_B,
            3)

    def move(self, x, y):
        """sets motors to move linearly with x and y in range [-1,1]"""
        self.top.set(x)
        self.bottom.set(-x)
        self.left.set(y)
        self.right.set(-y)

    def spin(self, speed):
        """turns at given speed in range [-1,1]"""
        self.top.set(speed)
        self.bottom.set(speed)
        self.left.set(speed)
        self.right.set(speed)

    def stop(self):
        self.move(0, 0)

    def __str__(self):
        return "Top: " + str(self.top) + "\nBottom: " + str(self.bottom) \
            + "\nLeft: " + str(self.left) + "\nRight: " + str(self.right)

    def __del__(self):
        self.stop()
        RPIO.PWM.cleanup()
        RPIO.cleanup()

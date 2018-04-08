import RPIO
from RPIO import PWM
from math import copysign

class Motor:
    def __init__(self, pin1, pin2, pwm_pin, encA, encB, dma_channel=0):
        self.dma_channel = dma_channel
        self.pwm_pin = pwm_pin
        self.pin1 = pin1
        self.pin2 = pin2
        self.encA = encA
        self.encB = encB
        self.speed = 0
        self.ticks = 0
        # Set up gpio pins, encoder interface, and dma
        PWM.init_channel(dma_channel, subcycle_time_us=8000)
        RPIO.setup(pwm_pin, RPIO.OUT)
        RPIO.setup(pin1, RPIO.OUT)
        RPIO.setup(pin2, RPIO.OUT)
        # todo: add encoder here

    def set(self, speed):
        """manipulates hardware to turn at given speed in range [-1,1] (positive
        should be clockwise)"""
        # todo: verify that positive is clockwise for each motor
        mysign = copysign(1, self.speed)
        newsign = copysign(1, speed)
        self.speed = speed
        if mysign != newsign:
            if newsign == 0:
                RPIO.output(self.pin1, False)
                RPIO.output(self.pin2, False)
            elif newsign == -1:
                RPIO.output(self.pin1, False)
                RPIO.output(self.pin2, True)
            elif newsign == 1:
                RPIO.output(self.pin2, False)
                RPIO.output(self.pin1, True)
        self.set_speed(abs(self.speed))

    def set_speed(self, speed):
        """adds a pwm pulse for the given speed in range [0,1]"""
        PWM.add_channel_pulse(self.dma_channel, self.pwm_pin, start=0, width=int(
            speed * PWM.get_channel_subcycle_time_us(self.dma_channel) / 10))

    def __str__(self):
        return "Speed: " + str(self.speed) + "\t\tTicks: " + str(self.ticks)

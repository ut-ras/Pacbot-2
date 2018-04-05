import RPIO
from RPIO import PWM
from math import copysign

TOP_PIN_1 = 23
TOP_PIN_2 = 24
TOP_PIN_PWM = 3
TOP_ENC_A = 0
TOP_ENC_B = 0

LEFT_PIN_1 = 7
LEFT_PIN_2 = 8
LEFT_PIN_PWM = 2
LEFT_ENC_A = 0
LEFT_ENC_B = 0

BOTTOM_PIN_1 = 11
BOTTOM_PIN_2 = 25
BOTTOM_PIN_PWM = 18
BOTTOM_ENC_A = 0
BOTTOM_ENC_B = 0

RIGHT_PIN_1 = 27
RIGHT_PIN_2 = 14
RIGHT_PIN_PWM = 17
RIGHT_ENC_A = 0
RIGHT_ENC_B = 0

RPIO.cleanup()
PWM.cleanup()

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
        # Set up gpio pins, encoder interupts, and dma
        PWM.init_channel(dma_channel, subcycle_time_us=8000)
        RPIO.setup(pwm_pin, RPIO.OUT)
        RPIO.setup(pin1, RPIO.OUT)
        RPIO.setup(pin2, RPIO.OUT)
        RPIO.setup(encA, RPIO.IN)
        RPIO.setup(encB, RPIO.IN)
        #  RPIO.add_interrupt_callback(
        #      encA,
        #      self.encCallback,
        #      edge='rising',
        #      threaded_callback=False)  # todo: see if this makes a difference

    def encCallback(self, gpioID, val):
        temp = RPIO.input(self.encB)
        if temp:
            self.ticks += 1
        else:
            self.ticks += 1
        #  print(self.ticks)

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
        """adds a pwm pulse for the given speed in range [-1,1]"""
        PWM.add_channel_pulse(self.dma_channel, self.pwm_pin, start=0, width=int(
            speed * PWM.get_channel_subcycle_time_us(self.dma_channel) / 10))

    def __str__(self):
        return "Speed: " + str(self.speed) + "\t\tTicks: " + str(self.ticks)


class Base:

    def __init__(self):
        #  self.disable()   # is this needed?
        PWM.setup()
        self.top = Motor(TOP_PIN_1, TOP_PIN_2, TOP_PIN_PWM, TOP_ENC_A, TOP_ENC_B, 0)
        self.bottom = Motor(BOTTOM_PIN_1, BOTTOM_PIN_2, BOTTOM_ENC_A, BOTTOM_ENC_B,
                BOTTOM_PIN_PWM, 1)
        self.left = Motor(LEFT_PIN_1, LEFT_PIN_2, LEFT_PIN_PWM, LEFT_ENC_A,
                LEFT_ENC_B, 2)
        self.right = Motor(RIGHT_PIN_1, RIGHT_PIN_2, RIGHT_PIN_PWM, RIGHT_ENC_A,
                RIGHT_ENC_B, 3)

    def move(self, x, y):
        """sets motors to move linearly with x and y in range [-1,1]"""
        self.top.set(x)
        self.bottom.set(-x)
        self.left.set(y)
        self.right.set(-y)

    def rotate(self, speed):
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
        PWM.cleanup()
        RPIO.cleanup()


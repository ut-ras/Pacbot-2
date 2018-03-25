import RPIO
from RPIO import PWM
import time

TOP_PIN_1 = 23
TOP_PIN_2 = 24
TOP_PIN_PWM = 3

LEFT_PIN_1 = 7
LEFT_PIN_2 = 8
LEFT_PIN_PWM = 2

BOTTOM_PIN_1 = 11
BOTTOM_PIN_2 = 25
BOTTOM_PIN_PWM = 18

RIGHT_PIN_1 = 27
RIGHT_PIN_2 = 14
RIGHT_PIN_PWM = 17


class Motor:
    def __init__(self, pin1, pin2, dma_pwm, dma_channel=0):
        self.dma_channel = dma_channel
        self.dma_pwm = dma_pwm
        self.pin1 = pin1
        self.pin2 = pin2

        PWM.init_channel(dma_channel, subcycle_time_us=8000)
        RPIO.setup(dma_pwm, RPIO.OUT)
        RPIO.setup(pin1, RPIO.OUT)
        RPIO.setup(pin2, RPIO.OUT)

    def set_motor(self, mode):
        if mode == 0:
            RPIO.output(self.pin1, False)
            RPIO.output(self.pin2, False)
        elif mode == -1:
            RPIO.output(self.pin1, False)
            RPIO.output(self.pin2, True)
        elif mode == 1:
            RPIO.output(self.pin2, False)
            RPIO.output(self.pin1, True)
        else:
            raise ValueError('Invalid motor mode')

    def set_speed(self, percent_speed):
        PWM.add_channel_pulse(self.dma_channel, self.dma_pwm, start=0, width=self.calc_width(percent_speed))

    def calc_width(self, percent_on):
        if percent_on < 0 or percent_on > 100:
            raise ValueError('Percent speed must be between 0-100')
        print(PWM.get_channel_subcycle_time_us(self.dma_channel))
        print(int(((percent_on / 100) * (PWM.get_channel_subcycle_time_us(self.dma_channel))) // 10))
        return int(((percent_on / 100) * (PWM.get_channel_subcycle_time_us(self.dma_channel))) // 10)


class Base:

    def __init__(self):
        self.disable()
        PWM.setup()
        self.motorTop = Motor(TOP_PIN_1, TOP_PIN_2, TOP_PIN_PWM, 0)
        self.motorBottom = Motor(BOTTOM_PIN_1, BOTTOM_PIN_2, BOTTOM_PIN_PWM, 1)
        self.motorLeft = Motor(LEFT_PIN_1, LEFT_PIN_2, LEFT_PIN_PWM, 2)
        self.motorRight = Motor(RIGHT_PIN_1, RIGHT_PIN_2, RIGHT_PIN_PWM, 3)

    def move_dir_time(self, x_percent_speed, y_percent_speed, time_sec):
        self.move_dir(x_percent_speed, y_percent_speed)
        time.sleep(time_sec)
        self.stop()

    def move_dir(self, x_percent_speed, y_percent_speed):
        if x_percent_speed < 0:
            self.motorTop.set_motor(1)
            self.motorBottom.set_motor(-1)
        elif x_percent_speed > 0:
            self.motorTop.set_motor(-1)
            self.motorBottom.set_motor(1)
        else:
            self.motorTop.set_motor(0)
            self.motorBottom.set_motor(0)
        self.motorTop.set_speed(abs(x_percent_speed))
        self.motorBottom.set_speed(abs(x_percent_speed))

        if y_percent_speed < 0:
            self.motorLeft.set_motor(1)
            self.motorRight.set_motor(-1)
        elif y_percent_speed > 0:
            self.motorLeft.set_motor(-1)
            self.motorRight.set_motor(1)
        else:
            self.motorLeft.set_motor(0)
            self.motorRight.set_motor(0)
        self.motorLeft.set_speed(abs(y_percent_speed))
        self.motorRight.set_speed(abs(y_percent_speed))

    def move(self, direction, percent_speed):
        if direction == 'left':
            self.motorTop.set_speed(percent_speed)
            self.motorBottom.set_speed(percent_speed)

            self.motorRight.set_motor(0)
            self.motorLeft.set_motor(0)
            self.motorTop.set_motor(1)
            self.motorBottom.set_motor(-1)

        elif direction == 'right':
            self.motorTop.set_speed(percent_speed)
            self.motorBottom.set_speed(percent_speed)

            self.motorLeft.set_motor(0)
            self.motorRight.set_motor(0)
            self.motorTop.set_motor(-1)
            self.motorBottom.set_motor(1)

        elif direction == 'up':
            self.motorLeft.set_speed(percent_speed)
            self.motorRight.set_speed(percent_speed)

            self.motorTop.set_motor(0)
            self.motorBottom.set_motor(0)
            self.motorLeft.set_motor(1)
            self.motorRight.set_motor(-1)

        elif direction == 'down':
            self.motorLeft.set_speed(percent_speed)
            self.motorRight.set_speed(percent_speed)

            self.motorTop.set_motor(0)
            self.motorBottom.set_motor(0)
            self.motorLeft.set_motor(-1)
            self.motorRight.set_motor(1)

        else:
            raise ValueError('Invalid move direction')

    def stop(self):
        self.motorTop.set_motor(0)
        self.motorLeft.set_motor(0)
        self.motorBottom.set_motor(0)
        self.motorRight.set_motor(0)

    def disable(self):
        PWM.cleanup()
        RPIO.cleanup()

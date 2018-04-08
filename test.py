import time
import motor
import RPIO
from RPIO import PWM

RPIO.setwarnings(False)

PWM.setup()
testMotor = motor.Motor(17, 27, 22, 0, 0)

speed = 0.6
dur = 2

testMotor.set(speed)
time.sleep(dur)
testMotor.set(0)
testMotor.set(-speed)
time.sleep(dur)
testMotor.set(0)


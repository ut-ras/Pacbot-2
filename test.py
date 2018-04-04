import time
import motor
import RPIO
from RPIO import PWM

RPIO.setwarnings(False)

# non blocking interrupt enable
RPIO.wait_for_interrupts(threaded=True)


RPIO.cleanup()
PWM.cleanup()
PWM.setup()
testMotor = motor.Motor(17, 27, 22, 23, 24)

speed = 0.8
dur = 2

print(testMotor)
testMotor.set(speed)
time.sleep(dur)
testMotor.set(0)
print(testMotor)
testMotor.set(-speed)
time.sleep(dur)
testMotor.set(0)
print(testMotor)


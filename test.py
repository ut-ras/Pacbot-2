import time
import motor
import RPIO
from RPIO import PWM

RPIO.setwarnings(False)

# non blocking interrupt enable
#  RPIO.wait_for_interrupts(threaded=True)


PWM.setup()
testMotor = motor.Motor(17, 27, 22, 23, 24)

speed = 0.6
dur = 2

#  while True:
    #  pass

print(testMotor)
testMotor.set(speed)
time.sleep(dur)
testMotor.set(0)
print(testMotor)
testMotor.set(-speed)
time.sleep(dur)
testMotor.set(0)
print(testMotor)
import time
while True:
    time.sleep(1)

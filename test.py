import time
import motor

testMotor = motor.Motor(23, 24, 3, 0, 0)

print(testMotor)
testMotor.set(.6)
time.sleep(1)
testMotor.set(0)
print(testMotor)
time.sleep(1)
testMotor.set(-.6)
time.sleep(1)
testMotor.set(0)
print(testMotor)


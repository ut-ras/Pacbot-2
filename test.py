from motor import Base
import time

base = Base()

# base.move('left', 50)
# time.sleep(1)
# base.stop()
#
# base.move('down', 50)
# time.sleep(1)
# base.stop()
#
# base.move('right', 50)
# time.sleep(1)
# base.stop()
#
# base.move('up', 50)
# time.sleep(1)
# base.stop()



# base.motorTop.set_motor(1)
# base.motorTop.set_speed(60)
# time.sleep(1)
# base.motorTop.set_motor(0)
# base.motorLeft.set_motor(1)
# base.motorLeft.set_speed(60)
# time.sleep(1)
# base.motorLeft.set_motor(0)
# base.motorBottom.set_motor(1)
# base.motorBottom.set_speed(60)
# time.sleep(1)
# base.motorBottom.set_motor(0)
# base.motorRight.set_motor(1)
# base.motorRight.set_speed(60)
# time.sleep(1)
# base.motorRight.set_motor(0)
# base.disable()


base.motorTop.set_motor(1)
base.motorTop.set_speed(60)
base.motorLeft.set_motor(1)
base.motorLeft.set_speed(60)
base.motorBottom.set_motor(-1)
base.motorBottom.set_speed(60)
base.motorRight.set_motor(-1)
base.motorRight.set_speed(60)
time.sleep(5)
base.stop()
base.disable()
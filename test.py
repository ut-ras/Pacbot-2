import hal
import sys

hal.initRobot()

hal.hardStop()

import time

# 0 1 2 3
# r l d u
if False:
    hal.moveDir(6, 0)
    hal.moveDir(2, 0)
    time.sleep(.5)
    hal.moveDir(15, 3)
    hal.hardStop()
    sys.exit()

hal.moveDir(6, 1)
time.sleep(.5)
hal.moveDir(14, 0)
#  time.sleep(.5)
hal.moveDir(3, 2)
#  time.sleep(.5)
hal.moveDir(5, 0)
#  time.sleep(.5)
hal.moveDir(3, 2)
#  time.sleep(.5)
hal.moveDir(10, 1)
#  time.sleep(.5)

#  while True:
#      time.sleep(1)
#      hal.printEnc()

hal.hardStop()
sys.exit()
# move in a square to test HAL
#  for i in range(4):
#      hal.moveDir(2, i)

hal.hardStop()

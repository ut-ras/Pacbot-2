import hal

hal.initRobot()

# move in a square to test HAL
for i in range(4):
    hal.moveDir(2, i)

hal.hardStop()

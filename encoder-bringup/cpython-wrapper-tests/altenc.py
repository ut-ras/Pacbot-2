#  import RPIO
import time
#  import wiringpi
import cenc

cenc.init_stuff()

while True:
    print(cenc.getTicks())
    time.sleep(.5)


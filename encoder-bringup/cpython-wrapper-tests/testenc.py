import time
import enc

enc.initChannel(0, 4, 5)

enc.initChannel(1, 10, 11)

while True:
    print(enc.getTicks(0),'\t\t',enc.getTicks(1))
    time.sleep(.5)


import wiringpi as wp
from time import sleep

A = 23
B = 24

count = 0
state = 0

cmax = -9999
cmin =  9999

""" STOLEN """
def pinISRB():
    global count
    count += 1

def pinISR():
    global count, state, cmin, cmax

    (a, b) = (wp.digitalRead(A), wp.digitalRead(B))

    # We're in one of four of states:
    s = state & 3

    # Update state, new state:
    s |= (a * (4) | b * (8))
    state = (s >> 2)

    # React based on current state and next state:
    count +=   (s in (0b0001, 0b0111, 0b1000, 0b1100))
    count -=   (s in (0b0010, 0b0100, 0b1011, 0b1101))
    count += 2*(s in (0b0011, 0b1010))
    count -= 2*(s in (0b0110, 0b1001))

    # If 0 or 5 or 10 or 15: HCF

    (cmax, cmin) = max(count, cmax), min(count, cmin)

wp.wiringPiSetupGpio()

wp.pinMode(A, wp.GPIO.INPUT)
wp.pinMode(B, wp.GPIO.INPUT)

wp.pullUpDnControl(A, wp.GPIO.PUD_UP)
wp.pullUpDnControl(B, wp.GPIO.PUD_UP)

wp.wiringPiISR(A, wp.GPIO.INT_EDGE_BOTH, pinISR)
wp.wiringPiISR(B, wp.GPIO.INT_EDGE_BOTH, pinISR)


while True:
#    sleep(0.5)
    wp.delay(500)
    print("Count: ", count, "\t\t Max: ", cmax, "\t\t Min: ", cmin)

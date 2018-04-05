import wiringpi
PIN_TO_SENSE = 23

count = 0

def gpio_callback():
    global count
    count += 1
#    print("GPIO_CALLBACK!")

wiringpi.wiringPiSetupGpio()
wiringpi.pinMode(PIN_TO_SENSE, wiringpi.GPIO.INPUT)
wiringpi.pullUpDnControl(PIN_TO_SENSE, wiringpi.GPIO.PUD_UP)

wiringpi.wiringPiISR(PIN_TO_SENSE, wiringpi.GPIO.INT_EDGE_BOTH, gpio_callback)

while True:
    wiringpi.delay(2000)
    print("Yo")
    print(count)

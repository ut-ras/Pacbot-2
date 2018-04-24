#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>

#include <RASLib/inc/pwm.h>

#include <stdint.h>

// Blink the LED to show we're on
tBoolean blink_on = true;

#define IN1 PIN_D1
#define IN2 PIN_D2

#define min(a, b) ((a) <= (b) ? (a) : (b))
#define max(a, b) ((a) >= (b) ? (a) : (b))

tPWM *uno, *dos;
int8_t speed;

#define SCALE_LEN 5

static const char scale[2*SCALE_LEN + 1] = "asdfghjkl;'";


void blink(void)
{
    SetPin(PIN_F3, blink_on);
    blink_on = !blink_on;
}

void setOutputs(const int8_t set)
{
    // If set is below 0, go in direction of IN1:
    SetPWM(uno, -min(set/128.0, 0), 0);
    SetPWM(dos,  max(set/128.0, 0), 0);
}

int8_t getSpeed(char c)
{
    int8_t step  = 127 / (SCALE_LEN),
           speed = -(SCALE_LEN);

    while (speed <= SCALE_LEN)
    {
        if (scale[speed + SCALE_LEN] == c)
            break;

        speed++;
    }

    return speed * step;
}

int main(void)
{
    CallEvery(blink, 0, 0.5);

    InitializeUART(115200);

    uno = InitializePWM(IN1, 200);
    dos = InitializePWM(IN2, 200);

    setOutputs(0);
    int8_t set;


    while (true) {

        Printf("Speed? ");

        set = getSpeed(Getc());
        Printf("%d\n", set, set);
        Printf("%f & %f\n\n", -min(set/128.0, 0), max(set/128.0, 0));

        setOutputs(set);
    }
}

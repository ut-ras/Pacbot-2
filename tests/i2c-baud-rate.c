#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

int main(int argc, char** argv)
{
    FILE *fd = fopen("/sys/class/i2c-adapter/i2c-1/of_node/clock-frequency", "rt");

    uint8_t buf[4] = {0};
    uint32_t num = 0;
    fread(buf, 4, 1, fd);

    for (uint8_t i = 0; i < sizeof(buf); i++)
        num |= (buf[i] << (8*(sizeof(buf) - i - 1)));

    printf("%zu\n\n", sizeof(buf[0]));

    printf("Val? %d\n", num);

    for (int i = 0; i < 16; i++)
    {
        printf("At %d: %x\n", i, buf[i]);
    }
}


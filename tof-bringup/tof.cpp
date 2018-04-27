#include <pigpio.h>
#include <stdio.h>
#include "I2Cdev.h"

const short __VL6180X_IDENTIFICATION_MODEL_ID               = 0x0000;
const short __VL6180X_IDENTIFICATION_MODEL_REV_MAJOR        = 0x0001;
const short __VL6180X_IDENTIFICATION_MODEL_REV_MINOR        = 0x0002;
const short __VL6180X_IDENTIFICATION_MODULE_REV_MAJOR       = 0x0003;
const short __VL6180X_IDENTIFICATION_MODULE_REV_MINOR       = 0x0004;
const short __VL6180X_IDENTIFICATION_DATE                   = 0x0006;
const short __VL6180X_IDENTIFICATION_TIME                   = 0x0008;

const short __VL6180X_SYSTEM_MODE_GPIO0                     = 0x0010;
const short __VL6180X_SYSTEM_MODE_GPIO1                     = 0x0011;
const short __VL6180X_SYSTEM_HISTORY_CTRL                   = 0x0012;
const short __VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO          = 0x0014;
const short __VL6180X_SYSTEM_INTERRUPT_CLEAR                = 0x0015;
const short __VL6180X_SYSTEM_FRESH_OUT_OF_RESET             = 0x0016;
const short __VL6180X_SYSTEM_GROUPED_PARAMETER_HOLD         = 0x0017;

const short __VL6180X_SYSRANGE_START                        = 0x0018;
const short __VL6180X_SYSRANGE_THRESH_HIGH                  = 0x0019;
const short __VL6180X_SYSRANGE_THRESH_LOW                   = 0x001A;
const short __VL6180X_SYSRANGE_INTERMEASUREMENT_PERIOD      = 0x001B;
const short __VL6180X_SYSRANGE_MAX_CONVERGENCE_TIME         = 0x001C;
const short __VL6180X_SYSRANGE_CROSSTALK_COMPENSATION_RATE  = 0x001E;
const short __VL6180X_SYSRANGE_CROSSTALK_VALID_HEIGHT       = 0x0021;
const short __VL6180X_SYSRANGE_EARLY_CONVERGENCE_ESTIMATE   = 0x0022;
const short __VL6180X_SYSRANGE_PART_TO_PART_RANGE_OFFSET    = 0x0024;
const short __VL6180X_SYSRANGE_RANGE_IGNORE_VALID_HEIGHT    = 0x0025;
const short __VL6180X_SYSRANGE_RANGE_IGNORE_THRESHOLD       = 0x0026;
const short __VL6180X_SYSRANGE_MAX_AMBIENT_LEVEL_MULT       = 0x002C;
const short __VL6180X_SYSRANGE_RANGE_CHECK_ENABLES          = 0x002D;
const short __VL6180X_SYSRANGE_VHV_RECALIBRATE              = 0x002E;
const short __VL6180X_SYSRANGE_VHV_REPEAT_RATE              = 0x0031;

const short __VL6180X_SYSALS_START                          = 0x0038;
const short __VL6180X_SYSALS_THRESH_HIGH                    = 0x003A;
const short __VL6180X_SYSALS_THRESH_LOW                     = 0x003C;
const short __VL6180X_SYSALS_INTERMEASUREMENT_PERIOD        = 0x003E;
const short __VL6180X_SYSALS_ANALOGUE_GAIN                  = 0x003F;
const short __VL6180X_SYSALS_INTEGRATION_PERIOD             = 0x0040;

const short __VL6180X_RESULT_RANGE_STATUS                   = 0x004D;
const short __VL6180X_RESULT_ALS_STATUS                     = 0x004E;
const short __VL6180X_RESULT_INTERRUPT_STATUS_GPIO          = 0x004F;
const short __VL6180X_RESULT_ALS_VAL                        = 0x0050;
const short __VL6180X_RESULT_HISTORY_BUFFER                 = 0x0052;
const short __VL6180X_RESULT_RANGE_VAL                      = 0x0062;
const short __VL6180X_RESULT_RANGE_RAW                      = 0x0064;
const short __VL6180X_RESULT_RANGE_RETURN_RATE              = 0x0066;
const short __VL6180X_RESULT_RANGE_REFERENCE_RATE           = 0x0068;
const short __VL6180X_RESULT_RANGE_RETURN_SIGNAL_COUNT      = 0x006C;
const short __VL6180X_RESULT_RANGE_REFERENCE_SIGNAL_COUNT   = 0x0070;
const short __VL6180X_RESULT_RANGE_RETURN_AMB_COUNT         = 0x0074;
const short __VL6180X_RESULT_RANGE_REFERENCE_AMB_COUNT      = 0x0078;
const short __VL6180X_RESULT_RANGE_RETURN_CONV_TIME         = 0x007C;
const short __VL6180X_RESULT_RANGE_REFERENCE_CONV_TIME      = 0x0080;

const short __VL6180X_READOUT_AVERAGING_SAMPLE_PERIOD       = 0x010A;
const short __VL6180X_FIRMWARE_BOOTUP                       = 0x0119;
const short __VL6180X_FIRMWARE_RESULT_SCALER                = 0x0120;
const short __VL6180X_I2C_SLAVE_DEVICE_ADDRESS              = 0x0212;
const short __VL6180X_INTERLEAVED_MODE_ENABLE               = 0x02A3;

int enablePins[4] = {12, 23, 24, 24};
int sensors[4] = {0x29, 0x2a, 0x2b, 0x2c};

// first list is addresses, second is values
int initPairs[2][29] = {
    {
        0x208, 0x96, 0x97, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xf5, 0xd9,
        0xdb, 0xdc, 0xdd, 0x9f, 0xa3, 0xb7, 0xbb, 0xb2, 0xca, 0x198,
        0x1b0, 0x1ad, 0xff, 0x100, 0x199, 0x1a6, 0x1ac, 0x1a7, 0x30
    },
    {
        0x01, 0x00, 0xfd, 0x00, 0x04, 0x02, 0x01, 0x03, 0x02, 0x05,
        0xce, 0x03, 0xf8, 0x00, 0x3c, 0x00, 0x3c, 0x09, 0x09, 0x01,
        0x17, 0x00, 0x05, 0x05, 0x05, 0x1b, 0x3e, 0x1f, 0x00
    }
};

int WriteByte(unsigned addr, uint16_t reg, uint8_t data) {
    //printf("register: %i, data: %i\n", reg, data);
    return I2Cdev::writeBytes(addr, reg, 1, &data);

}

char ReadByte(unsigned addr, uint16_t reg) {
    uint8_t data_read;
    I2Cdev::readBytes(addr, reg, 1, &data_read);
    return data_read;
}

//reinitialize all distance sensors to address 0x29
void resetAddr() {
    for(int i = 0; i < 4; ++i) {
        gpioWrite(enablePins[i], 1);
    }
    for(int i = 0; i < 4; ++i) {
        WriteByte(sensors[i], __VL6180X_I2C_SLAVE_DEVICE_ADDRESS, 0x29);
    }
}

void initSensor(sensor) {
    //enable one sensor at a time
    printf("Sensor %d enabling\n", sensor);
    gpioWrite(enablePins[sensor], 1);
    //i2cReadByteData(sensor, __VL6180X_SYSTEM_FRESH_OUT_OF_RESET);
    // proprietary setup stuff
    for(int j = 0; j < 29; ++j) {
        WriteByte(0x29, initPairs[0][sensor], initPairs[1][sensor]);
    }
    // recommended setup stuff
    // WriteByte(sensor, __VL6180X_SYSTEM_MODE_GPIO1, 0x10);
    WriteByte(0x29, __VL6180X_READOUT_AVERAGING_SAMPLE_PERIOD, 0x30);
    WriteByte(0x29, __VL6180X_SYSALS_ANALOGUE_GAIN, 0x46);
    WriteByte(0x29, __VL6180X_SYSRANGE_VHV_REPEAT_RATE, 0xff);
    WriteByte(0x29, __VL6180X_SYSALS_INTEGRATION_PERIOD, 0x63);
    WriteByte(0x29, __VL6180X_SYSRANGE_VHV_RECALIBRATE, 0x01);
    // optional settings todo: add more settings
    WriteByte(0x29, 0x001b, 0x09);
    WriteByte(0x29, 0x003e, 0x31);
    WriteByte(0x29, 0x0014, 0x24);
    // assign new register
    WriteByte(0x29, __VL6180X_I2C_SLAVE_DEVICE_ADDRESS, sensors[sensor]);
    printf("finished sensor %d\n", i);
}

int main() {
    gpioInitialise();
    I2Cdev::initialize();
    I2Cdev::enable(true);
    resetAddr();
    //must open address that tof is currently on
    for(int i = 0; i < 4; ++i) {
        initSensor(i);
    }
    while(1) {
        for(int i = 0; i < 4; ++i) {
            // wait until ready
            //while(!ReadByte(sensor1, 0x004d) & (1<<0));
            // start measurment 1
            WriteByte(sensors[i], 0x0018, 0x01);
            //while(!ReadByte(sensor1, 0x004f) & (1<<2));
            int reading = ReadByte(sensors[i], 0x0062);
            printf("%04dmm\t", reading);
        }
        printf("\n");
    }
    return 0;
}


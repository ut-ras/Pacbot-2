#include <pigpio.h>
#include <stdio.h>

const short __VL6180X_IDENTIFICATION_MODEL_ID               = 0x0000;
const short __VL6180X_IDENTIFICATION_MODEL_REV_MAJOR        = 0x0001;
const short __VL6180X_IDENTIFICATION_MODEL_REV_MINOR        = 0x0002;
const short __VL6180X_IDENTIFICATION_MODULE_REV_MAJOR       = 0x0003;
const short __VL6180X_IDENTIFICATION_MODULE_REV_MINOR       = 0x0004;
const short __VL6180X_IDENTIFICATION_DATE                   = 0x0006;   //# 16bit value
const short __VL6180X_IDENTIFICATION_TIME                   = 0x0008;   //# 16bit value

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

int WriteByte(unsigned, uint16_t, char);

int sensor;
int main() {
    gpioInitialise();

    if((sensor = i2cOpen(1, 0x29, 0)) < 0) {
        printf("i2c open failed no 1, handle: %i\n", sensor);
        return 1;
    }
    i2cReadByteData(sensor, __VL6180X_SYSTEM_FRESH_OUT_OF_RESET);

    // proprietary setup stuff
    WriteByte(sensor, 0x0207, 0x01);
    WriteByte(sensor, 0x0208, 0x01);
    WriteByte(sensor, 0x0096, 0x00);
    WriteByte(sensor, 0x0097, 0xfd);
    WriteByte(sensor, 0x00e3, 0x00);
    WriteByte(sensor, 0x00e4, 0x04);
    WriteByte(sensor, 0x00e5, 0x02);
    WriteByte(sensor, 0x00e6, 0x01);
    WriteByte(sensor, 0x00e7, 0x03);
    WriteByte(sensor, 0x00f5, 0x02);
    WriteByte(sensor, 0x00d9, 0x05);
    WriteByte(sensor, 0x00db, 0xce);
    WriteByte(sensor, 0x00dc, 0x03);
    WriteByte(sensor, 0x00dd, 0xf8);
    WriteByte(sensor, 0x009f, 0x00);
    WriteByte(sensor, 0x00a3, 0x3c);
    WriteByte(sensor, 0x00b7, 0x00);
    WriteByte(sensor, 0x00bb, 0x3c);
    WriteByte(sensor, 0x00b2, 0x09);
    WriteByte(sensor, 0x00ca, 0x09);
    WriteByte(sensor, 0x0198, 0x01);
    WriteByte(sensor, 0x01b0, 0x17);
    WriteByte(sensor, 0x01ad, 0x00);
    WriteByte(sensor, 0x00ff, 0x05);
    WriteByte(sensor, 0x0100, 0x05);
    WriteByte(sensor, 0x0199, 0x05);
    WriteByte(sensor, 0x01a6, 0x1b);
    WriteByte(sensor, 0x01ac, 0x3e);
    WriteByte(sensor, 0x01a7, 0x1f);
    WriteByte(sensor, 0x0030, 0x00);

    // recommended setup stuff
    WriteByte(sensor, __VL6180X_SYSTEM_MODE_GPIO1, 0x10);
    WriteByte(sensor, __VL6180X_READOUT_AVERAGING_SAMPLE_PERIOD, 0x30);
    WriteByte(sensor, __VL6180X_SYSALS_ANALOGUE_GAIN, 0x46);
    WriteByte(sensor, __VL6180X_SYSRANGE_VHV_REPEAT_RATE, 0xff);
    WriteByte(sensor, __VL6180X_SYSALS_INTEGRATION_PERIOD, 0x63);
    WriteByte(sensor, __VL6180X_SYSRANGE_VHV_RECALIBRATE, 0x01);

    // optional settings
    // todo: add more settings


    //setting: set GPIO I2C addresses
    //process: device with i2c open (on addr 0x29)
    //then with handle obtained write to address 0x212 new address
    //reopen with i2c open?

    //writes to TOF register 0x212 (I2C_SLAVE_DEVICE_ADDRESS) 0x30 - new I2C addr
    int ec = (WriteByte(sensor, __VL6180X_I2C_SLAVE_DEVICE_ADDRESS, 0x30));
    if(ec != 0) {
        printf("i2c change address failed; exit code: %i\n", ec);
        return ec;
    } //returns error -82, check why
    if((sensor = i2cOpen(1, 0x30, 0)) < 0) {
        printf("i2c open failed no2, handle: %i\n", sensor);
        return 1;
    }


    int reading = 0;
    while(1) {
        // wait until ready
        while(!i2cReadByteData(sensor, 0x004d) & (1<<0));
        // start measurment
        WriteByte(sensor, 0x0018, 0x01);
        while(!i2cReadByteData(sensor, 0x004f) & (1<<2));
        reading = i2cReadByteData(sensor, 0x0062);
        printf("%04dmm\n", reading);
        time_sleep(.5);
    }

    i2cClose(sensor);
    return 0;
}

/**
 * Splits 16-bit register address into two bytes and writes address + data via I²C
 * LeoWhite (https://github.com/LeoWhite/OptimusPi/blob/master/Tests/VL6180Test.cpp)
 * repurposed to output status from write_device (Matthew Yu)\
 * param: handle, device handle (generated from i2cOpen funct call)
 * param: reg, 2 byte register addr
 * param: data, 1 byte data to write
 * return: 0 if OK, otherwise PI_BAD_HANDLE, PI_BAD_PARAM, or PI_I2C_WRITE_FAILED.
 **/
int WriteByte(unsigned handle, uint16_t reg, char data)
{
  char data_write[3];
  printf("register: %i, data: %s\n", reg, data);
  data_write[0] = (reg >> 8) & 0xFF;; // MSB of register address
  data_write[1] = reg & 0xFF; // LSB of register address
  data_write[2] = data & 0xFF;
  return i2cWriteDevice(handle, data_write, 3);
}

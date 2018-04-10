# Pacbot Hardware Access Layer

## Includes code for:
- Driving DC motors by PWM over an H-bridge
- Reading high frequency quadrature signals
- ~~Configuring and reading multiple VL6180X distance sensors~~
- ~~Reading the super secret DMP registers from the MPU6050~~
- ~~Running PID control loops~~
- ~~Interleaving python algorithms with C drivers~~
- ~~TCP protobuf interface~~

## Setup:
Download and install pigpio c library
```
cd ~
rm pigpio.zip
sudo rm -rf PIGPIO
wget abyz.me.uk/rpi/pigpio/pigpio.zip
unzip pigpio.zip
cd PIGPIO
make
sudo make install
```

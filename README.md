# pacbot Hardware Access Layer

## Features:
- [x] Drives DC motors over an H-Bridge with PWM
- [x] Reads high frequency quadrature signals
- [ ] Configures and reads from multiple VL6180X distance sensors
- [ ] Uses the super secret DMP on the MPU6050
- [ ] Runs PID control loops
- [ ] Has a shiny Python wrapper for higher level algorithms and logic
- ~~[ ] Exposes a TCP Interface~~

## Setup:
Download and install pigpio c library
```bash
cd ~
rm pigpio.zip
sudo rm -rf PIGPIO
wget abyz.me.uk/rpi/pigpio/pigpio.zip
unzip pigpio.zip
cd PIGPIO
make
sudo make install
```

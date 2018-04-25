# pacbot Hardware Access Layer

## Features:
- [x] Drives DC motors over an H-Bridge with PWM
- [x] Reads high frequency quadrature signals
- [ ] Configures and reads from multiple VL6180X distance sensors
- [ ] Uses the super secret DMP on the MPU6050
- [x] Runs PID control loops
- [x] Has a shiny Python wrapper for higher level algorithms and logic

## Setup:
Download and install pigpio c library
```bash
rm pigpio.zip
sudo rm -rf PIGPIO
wget abyz.me.uk/rpi/pigpio/pigpio.zip
unzip pigpio.zip
cd PIGPIO
make
sudo make install
```
Build and install HAL code as python library
```bash
git clone https://github.com/ut-ras/pb-HAL.git
cd pb-HAL
sudo python3 setup.py install
```

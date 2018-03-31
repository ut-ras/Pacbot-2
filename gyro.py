#!/usr/bin/python
import smbus
import math

# Register
power_mgmt_1 = 0x6b
power_mgmt_2 = 0x6c

def read_byte(reg):
    return bus.read_byte_data(address, reg)

def read_word(reg):
    h = bus.read_byte_data(address, reg)
    l = bus.read_byte_data(address, reg+1)
    value = (h << 8) + l
    return value

def read_word_2c(reg):
    val = read_word(reg)
    if (val >= 0x8000):
        return -((65535 - val) + 1)
    else:
        return val

def dist(a,b):
    return math.sqrt((a*a)+(b*b))

def get_y_rotation(x,y,z):
    radians = math.atan2(x, dist(y,z))
    return -math.degrees(radians)

def get_x_rotation(x,y,z):
    radians = math.atan2(y, dist(x,z))
    return math.degrees(radians)

bus = smbus.SMBus(1) # bus = smbus.SMBus(0) fuer Revision 1
address = 0x68       # via i2cdetect

# Aktivieren, um das Modul ansprechen zu koennen
bus.write_byte_data(address, power_mgmt_1, 0)

print ("Gyroskop")
print ("--------")

gyroskop_xout = read_word_2c(0x43)
gyroskop_yout = read_word_2c(0x45)
gyroskop_zout = read_word_2c(0x47)

print("gyroskop_xout: ", "%5d" % gyroskop_xout, " scaled: ", gyroskop_xout / 131)
print("gyroskop_yout: ", "%5d" % gyroskop_yout, " scaled: ", gyroskop_yout / 131)
print("gyroskop_zout: ", "%5d" % gyroskop_zout, " scaled: ", gyroskop_zout / 131)

print()
print("Accelerationsensor")
print("---------------------")

Acceleration_xout = read_word_2c(0x3b)
Acceleration_yout = read_word_2c(0x3d)
Acceleration_zout = read_word_2c(0x3f)

Acceleration_xout_scaled = Acceleration_xout / 16384.0
Acceleration_yout_scaled = Acceleration_yout / 16384.0
Acceleration_zout_scaled = Acceleration_zout / 16384.0

print("Acceleration_xout: ", "%6d" % Acceleration_xout, " scaled: ", Acceleration_xout_scaled)
print("Acceleration_yout: ", "%6d" % Acceleration_yout, " scaled: ", Acceleration_yout_scaled)
print("Acceleration_zout: ", "%6d" % Acceleration_zout, " scaled: ", Acceleration_zout_scaled)

print("X Rotation: ", get_x_rotation(Acceleration_xout_scaled, Acceleration_yout_scaled, Acceleration_zout_scaled))
print("Y Rotation: ", get_y_rotation(Acceleration_xout_scaled, Acceleration_yout_scaled, Acceleration_zout_scaled))

"""
Sample Data Output Expected
Gyroskop
--------
gyroskop_xout:   -260  scaled:  -2
gyroskop_yout:   -154  scaled:  -2
gyroskop_zout:     78  scaled:  0

Accelerationsensor
---------------------
Acceleration_xout:   -1048  scaled:  -0.06396484375
Acceleration_yout:    -676  scaled:  -0.041259765625
Acceleration_zout:   16644  scaled:  1.01586914062
X Rotation:  -2.32121150537
Y Rotation:  3.59994842011
"""

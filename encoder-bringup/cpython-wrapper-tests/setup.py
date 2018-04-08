from distutils.core import setup, Extension

encModule = Extension('enc',
                      sources=['enc.c'],
                      libraries=['wiringPi'],)

setup(name='Encoder',
      version='1.0',
      description='Reads encoders with hardware interrupts',
      ext_modules=[encModule])

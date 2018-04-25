from distutils.core import setup, Extension

module1 = Extension('hal',
                    sources=['src/main.cpp', 'src/enc.cpp',
                             'src/motor.cpp', 'src/pid.cpp'],
                    libraries=['pigpio'],)

setup(name='pb-HAL',
      version='1.0',
      description='Pacbot Hardware Access Layer',
      ext_modules=[module1])

BFMC Embedded Platfrom
=======================

This documentation describes the low level application, which runs on the micro-controller Nucleo-F401RE used on the challenge BFMC. It aims to control the robot movement 
and provide a interface between a higher level controllers and lower level actuators, sensors. The current version uses mbed 2 library and mbed 2 rtos library.
The project has five main part: brain, hardware, signal, utils, examples. The brain folder contains the main state machine of robot, the drivers for actuator and sensors
are implemented in the 'hardware' folder. The 'signal' folder  includes libraries for processing signals. 'Utils' package incorporates some util functionalities. 
Some simple usages of these are instanced in the 'examples' folder. 

[Documentation](https://bfmcembeddedplatform.readthedocs.io/en/stable/) 
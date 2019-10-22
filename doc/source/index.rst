Welcome to BFMC Embedded Platform's documentation!
==================================================

This documentation describes the low level application, which runs on the micro-controller Nucleo-F401RE. It aims to control the robot movement 
and provide a interface between a higher level controllers and lower level actuators, sensors. The current version uses mbed 2 library and mbed 2 rtos library.
The project has five main part: brain, hardware, signal, utils, examples. The brain folder contains the main state machine of robot, the drivers for actuator and sensors
are implemented in folder hardware. Folder signal include libraries for processing signals. Utils incorporates some util functionalities, some simple usages of these are instanced 
in folder examples. 

.. toctree::
   :maxdepth: 2
   :caption: Contents:
    
   hardware/index
   signal/index
   utils/index
   examples/index


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

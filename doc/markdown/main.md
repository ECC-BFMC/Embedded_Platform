Main Page {#mainpage}
=========
The "RcCar Nucleo Soft." is a embedded software, which runs on the Nucleo F401RE for controlling the actuator and the sensors. This application based on the Mbed OS library amd Mbed-rtos library. This application communicates with the other single board computer (SCB), for example a Raspberry Pi, over the UART serial commination.

Tutorial
------
<!-- We have to remane the file. -->
@subpage markdown/basemain.md The basic main.cpp source file description \n
@subpage markdown/encoder_and_pid.md Create a quadrature encoder driver and pid controller
<!-- Safety stop -->
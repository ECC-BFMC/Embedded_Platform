This short tutorial presents the simple main source file, which contains a few simple functionality, like blinking the built-in led. It describe a simple version of the main source file, which will be completed with the other functionality in the next tutorials. \n
Such other C/C++ source file, it starts with the list of the included files, which contain the classes and the variable types.
@snippet main_ex0.cpp Include mbed
Where the mbed header file has all the high level declarations to use the mbed os library. It must be included if we want to base our application on the mbed os. In the next step it needs to include some of the our header files, which describe the main program components.
@snippet main_ex0.cpp List of included files
@dontinclude main_ex0.cpp
After including the header files, you need to create the serial object, which is the communication channel between the single board computer and the micro-controller based on the UART serial communication. The construction has two input parameter, the transmitter (TX) and the receiver (RX) pins. You can choose the USB port, in that case pins are predefined in the mbed library.
@skipline g_rpi(USBTX, USBRX)
In the next step, you need to declare the pins, by which you can control servo motor and the VNH motor driver, where the all parameters represent the pin on the board. The first pin(D9) generate the PWM signal (Pulse-width modulation) to set the position of the servo motor, the rest of the pins are used fot the VNH motor driver, where the first (D3) generate another pwm signal to control the voltage of the motor. The second two pins (D2 and D4) are digital output pins to set the state of the VNH driver, the state of the VNH driver determine the rotary direction of the motor. The last pin (A0) is an analog input pin, you can it use to read the current value. 
@skipline g_car(
You can initialize a simple led blinker object by the next line of code, this object is a periodically applied task, which change state of the output pin with the given frequency, where first parameter and the second parameter is the period of the toggling and the output pin, respectively. It's very useful to verify the correct continuos code execution.
@skipline  g_blinker
In the next step to control the actuator, you can set up the motion controller object, this object is a state machine to manage the state of the robots and to execute the given commands from single board computer. It must to have the same period as the encoder object if you want to apply a PID control. The second parameter is the serial interface, over which is sent the messages, you have to specify the move driver object and the control object (optional).
@skipline g_motionController(

CSerialMonitor is a class to handle the received messages from another device over the serial interface, it receive and process the messages. Each messages can be separate to two parts, key part and content, and are a specified format, defined in the following lines:

    #KKKK:content....;;\r\n

The first character ("#" or "@") symbolize the direction of the message, the message was received or sent. The key part of the messages is formed by four character, these keys have to differ for each different functionality, like MCTL for moving or BRAK for braking. These keys can you help to redirect the content of the message. The second part is composed by the content, and a message have to terminate with the same character combination,(;;\\r\\n). In our case each variable in the second part are separated by ";". You need to make a list of the pair, where each keys are attached to a callback function, which process the content of the messages. Some example messages:

    #MTCL:10.0;10.0;;\r\n
    #BRAK:10.0;;\r\n
    #SPLN:1;0.0;0.0;0.0;0.10;0.10;0.10;0.0,0.0;5.0;;\r\n

The list of the pair  and the serial monitor object you can declare by the following few line:
@snippet main_ex0.cpp List of messages
If you want to apply your method periodically, then you can use the task manager and the task based object. The task manager are functionality to apply the method in the given period, but all functionality have to the part to the derived class of the CTask base class, you can describe the method in the "void _run()" function, for example the "CEncoderSender" class. The first parameter of the base class is the number of the period, when the task is in the waiting state, you can calculate easily, the required period of the task have to divide by the  period of the task managed, named "g_baseTick". To initialize the task manager you have to enumerate the tasks, like in the next section:
@snippet main_ex0.cpp List of task
In the setup function you can initialize the objects, like the serial communication and start the RTos timers, in this function you can set the frequency of the serial interface and the start up message. The main function contains the main loop, which apply the loop function, this function applies main callback function of the task manager. 
In the next section you can read a simple example without quadrature encoder, PID controller or other functionality added.
\includelineno examples/main_ex0.cpp



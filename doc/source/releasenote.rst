Release notes
=============

Release date: 11.11.2019

Improvements
""""""""""""""" 
    - The baud rate of serial communication was reduced from 460800 bit/s to 256000 bit/s, for minimizing the packet loss between the single board computer and micro-controller. 
    - Representation of Move command was changed from percentage [-100%,100%] to ratio [-1,+1]. So the first value of 'Move' command can be duty cycle value in ratio expressed or the reference speed in meter/sec(mps). 
    - The BezierSpline and SafetyStop control mechanism was removed for code simplifying. 
    - We removed Move class and updated the interfaces (IMotorCommand,ISteeringCommand) for actuator drivers. 
    - We deleted the static callback functions and updated the callback objects' initialization.
    - We added the ranges and range verification to steering motor ([-23,23] in degree), dc motor ([-0.5,0.5] in ratio) drivers and motor controller ([-255,255] in rps). 
    - We done a code refactoring by renaming, adding namespaces and regrouping classes:
        - new namespace 'brain'
        - CMotionController -> CRobotStateMachine
        - CControllerSiso -> CMotorController
        - quadratureencoder -> quadraturecounter (header and source files)
        - quadratureencodertask -> quadratureencoder (header and source files)
        - servo -> steeringmotor (header and source files)
        - vnh -> dcmotor (header and source files)
        - template source file format change to .tpp
    - We deleted unused headers and source files:
        - commandinterpreter
        - encoder and encoderfilter
        - proximitypublisher
        - ackermannmodel
        - kalmanfilters
        - ekf_am
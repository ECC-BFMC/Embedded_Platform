# Upgrade and Tasks
This file enumerates some applicable new changes in the project. 
- H - high important
- L - low important
- U - urgent task
- N - non-urgent task
- O - optional feature



## OS
- [ ] (HN)Upgrade the OS version (2->5)
- [ ] (HN)Check the unnecessary files and folder (ex. like Queue)


## Serial communication
- [ ] (LN)New optimized communication protocol or applied other communication protocol
 - CAN, ROS2 (MicroROS) -> depends group interest and Micro-controller type. 
- [ ] (HN)Thread safe and interrupted safe write mechanism for sender buffer.
- [ ] (HN)Interface definition for writing. 

## Motor Driver
- [ ] (HU)New dc motor driver instead VNH5019 (acquisition problems)
- [ ] (LO)New brushless motor and driver

## Motor Controller
- [ ] (LO)Position controller
- [ ] (LO)Cascade controller
 - Speed + Current
 - Position + Speed + Current (Possible impl. higher level controller)
 - Position + Speed (Possible impl. higher level controller)

## Steering
- [ ] (LN)Precise position control
 - (LO)Time delay -> Transition (Possible impl. higher level controller)
 - (LN)Mechanic transfer (input and real output angle) -> Linear

## IMU - localization 
- [ ] (LO)localization filter based vehicle model (Extended Kalman Filter)
- [ ] (LO)particle filter -> easier to implement, but require more computational process.
- [ ] (LO)fusion with other sensor.... (only higher level implementation)
## Overall position control
- [ ] (LO)a higher level position control, taking in acount 
 - steering transfer function 
 - motor speed transfer function
 - feedback loop a IMU to localization
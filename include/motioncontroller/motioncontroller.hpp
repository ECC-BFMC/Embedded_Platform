/**
  ******************************************************************************
  * @file    MotionController.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class declaration for the motion controller
  *          functionality.
  ******************************************************************************
 */

#ifndef MOTION_CONTROLLER_HPP
#define MOTION_CONTROLLER_HPP

/* The mbed library */
#include <mbed.h>
#include <rtos.h>

#include <taskmanager/taskmanager.hpp>
#include <drivers/dcmotor.hpp>
#include <drivers/steeringmotor.hpp>

#include <move/move.hpp>

#include <controllers/motorcontroller.hpp>



/**
 * @brief Limit of the pwm command [percentage]
 * 
 */
#define MCTL_PWM_COMMAND_LIMIT 75
/**
 * @brief Limit of the linear velocity command [meter per second]
 * 
 */
#define MCTL_SPEED_COMMAND_LIMIT 1.5

/**
 * @brief CMotionController targets to implement the main state machine to control
 *  movement of robot and provide the interfaces to control functionality, like braking and moving. 
 * 
 */
class CMotionController 
{
public:

    CMotionController(
        float f_period_sec, 
        Serial& f_serialPort, 
        drivers::IMotorCommand&                 f_motorControl,
        drivers::ISteeringCommand&              f_steeringControl,
        controllers::CControllerSiso*           f_control = NULL);
    

    /* Start the Rtos timer for applying "_run" method  */
    void startRtosTimer();

    /* Serial callback method for moving */ 
    void serialCallbackMove(char const * a, char * b);
    /* Serial callback method for braking */
    void serialCallbackBrake(char const * a, char * b);
    /* Serial callback method for activating pid */
    void serialCallbackPID(char const * a, char * b);

    /* Reset method */
    void reset();
    /* Get speed method */
    float getSpeed();
    /* Get angle method */
    float getAngle();
    /* Callback for changing the state to brake.*/
    void BrakeCallback();
    /* Set state method */
    void setState(int f_state);
private:
    /* Contains the state machine, which control the lower level drivers (motor and steering) based the current state. */
    virtual void _run();
    
    /* Serial callback for a hard braking */
    void serialCallbackHardBrake(char const * a, char * b);
    
    
    
    /* Static function to convert from linear velocity ( centimeter per second ) of robot to angular velocity ( rotation per second ) of motor */
    static float Mps2Rps(float f_vel_cmps);

private:
    /* reference to Serial object */
    Serial& m_serialPort;
    /* Motor control interface */
    drivers::IMotorCommand&                 m_motorControl;
    /* Steering wheel control interface */
    drivers::ISteeringCommand&              m_steeringControl;
    /* Speed */
    float m_speed;
    /* Angle */
    float m_angle;
    /* PEriod i nseconds */
    float   m_period_sec;
    /* State machine state */
    uint8_t m_state;
    /* PID activation state */
    bool    m_ispidActivated;
    // 0-none
    // 1-normal
    // 2-brake regeneration
    
    /* Timeout for a hard braking with deactivated pid.  */
    Timeout                                 m_hbTimeOut;
    /* Speed Control for dc motor */
    controllers::CControllerSiso*           m_control;
    /* Rtos  timer for periodically applying */
    RtosTimer                               m_timer;
};

#endif // MOTION_CONTROLLER_H 

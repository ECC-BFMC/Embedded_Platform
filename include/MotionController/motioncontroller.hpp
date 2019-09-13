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
#include <TaskManager/taskmanager.hpp>
#include <Move/move.hpp>
#include <BezierMotionPlanner/bezierMotionplanner.hpp>
#include <Controllers/controller.hpp>
#include <SafetyStop/safetystopfunction.hpp>

#include <rtos.h>

/**
 * @brief Limit of the pwm command [percentage]
 * 
 */
#define MCTL_PWM_COMMAND_LIMIT 75
/**
 * @brief Limit of the velocity command [Centimeter per second]
 * 
 */
#define MCTL_SPEED_COMMAND_LIMIT 100

//!  CMotionController class. 
/*!
 *  It inherits class task::CTask. 
 *  It is used for executing move commands.
 * */
class CMotionController 
{
public:
    CMotionController(
        float f_period_sec,
        Serial& f_serialPort, 
        Move& f_car,
        CSafetyStopFunction* f_safetyStop,
        controllers::CControllerSiso* f_control = NULL);
    CMotionController(
        float f_period_sec, 
        Serial& f_serialPort, 
        Move& f_car,
        controllers::CControllerSiso*           f_control = NULL);
    

    /* Serial callback method for Move command */
    static void staticSerialCallbackMove(void* obj,char const * a, char * b);
    /* Serial callback method for BRAKE command */
    static void staticSerialCallbackBrake(void* obj,char const * a, char * b);
    /* Serial callback method for hard BRAKE command */
    static void staticSerialCallbackHardBrake(void* obj,char const * a, char * b) ;
    /* Serial callback method for PID activation command */
    static void staticSerialCallbackPID(void* obj,char const * a, char * b);
    /* Static serial callback for spline command */
    static void staticSerialCallbackSpline(void* obj,char const * a,char * b );
    /* Static callback function for run method */
    static void staticCallbackRun(void* obj);
    /* Start the Rtos timer for applying run */
    void startRtosTimer();

    /* Reset method */
    void reset();
    /* Get speed method */
    float getSpeed();
    /* Get angle method */
    float getAngle();
    /* BRAKE callback method */
    void BrakeCallback();
    /* Set state method */
    void setState(int f_state);
private:
    /* Run method */
    virtual void _run();
    /* Serial callback implementation */ 
    void serialCallbackMove(char const * a, char * b);
    /* BRAKE serial callback */
    void serialCallbackBrake(char const * a, char * b);
    /* Hard BRAKE serial callback */
    void serialCallbackHardBrake(char const * a, char * b);
    /* PID serial callback */
    void serialCallbackPID(char const * a, char * b);
    /* Spline serial callback */
    void serialCallbackSpline(char const * a, char * b);
    
    
    /* Static function to convert from linear velocity ( centimeter per second ) of robot to angular velocity ( rotation per second ) of motor */
    static float Mps2Rps(float f_vel_cmps);
    // /* Velocity to PWM function */
    // static float VEL2PWM(float vel);

private:
    /* reference to Serial object */
    Serial& m_serialPort;
    /* reference to MOVE object */
    Move& m_car;
    /* Speed */
    float m_speed;
    /* Angle */
    float m_angle;
    /* PEriod i nseconds */
    float   m_period_sec;
    /* Spline activation state */
    bool    m_isSplineActivated;
    /* State machine state */
    uint8_t m_state;
    /* PID activation state */
    bool    m_ispidActivated;
    // 0-none
    // 1-normal
    // 2-brake regeneration
    
    /* motion planner object */
    planner::CBezierMotionPlanner           m_motionPlanner;
    /* Timeout */
    Timeout                                 m_hbTimeOut;
    /* Reference to control object */
    controllers::CControllerSiso*           m_control;
    /* Reference to safety stop function */
    CSafetyStopFunction*                    m_safetyStop;
    /* Rtos  timer for periodically applying */
    RtosTimer                               m_timer;
};

#endif // MOTION_CONTROLLER_H 

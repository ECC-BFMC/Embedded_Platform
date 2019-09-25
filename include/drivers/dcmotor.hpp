/**
  ******************************************************************************
  * @file    VNH.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-2017
  * @brief   This file contains the class declaration for the VNH Bridge Driver
  *          functionality.
  ******************************************************************************
 */

/* Inclusion guard */
#ifndef DCMOTOR_HPP
#define DCMOTOR_HPP

/* The mbed library */
#include <mbed.h>
/* Functions to compute common mathematical operations and transformations */
#include <cmath>

namespace drivers{
    /**
     * @brief Current getter interface
     * 
     */
    class ICurrentGetter{
        public:
            virtual float getCurrent() = 0;
    };

    /**
     * @brief Command setter interface.
     * 
     */
    class IMotorCommand{
        public:
            virtual void setSpeed(float f_pwm) = 0;
            virtual void brake() = 0;
            virtual void inverseDirection(float f_pwm) = 0;
    };

    /**
     * @brief VNH Motor Driver class. 
     * 
     * It has role to control the vnh5019 motor driver. 
     * It has two interface, one to control the motor speed and direction and one to get the measured current value. 
     * 
     */
    class CMotorDriverVnh:public ICurrentGetter, public IMotorCommand
    {
    public:
        /* Constructor */
        CMotorDriverVnh(PinName, PinName, PinName, PinName);
        /* Destructor */
        ~CMotorDriverVnh();
        /* Run */
        void setSpeed(float f_pwm);
        /* Brake */
        void brake();
        /* Inverse */
        void inverseDirection(float f_pwm);
        /* Get current */
        float getCurrent();
        
    private:
        /* PWM output pin */
        PwmOut      m_pwm;
        /* pin A for direction */
        DigitalOut  m_ina;
        /* pin B for direction */
        DigitalOut  m_inb;
        /* Measured current value by driver */
        AnalogIn    m_current_in;
    };


}; // namespace drivers


#endif
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

namespace hardware::drivers{
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
            virtual bool inRange(float f_pwm) =0;
    };

    /**
     * @brief VNH5019 Motor Driver class 
     * 
     * It has role to control the vnh5019 motor driver. 
     * It has two interface, one to control the motor speed and direction and one to get the measured current value.
     * The direction and the rotation speed are conducted a value, where the positive values define the forward move, 
     * similary negative values represents the backward move. The magnitude of values gives the motor speed. So generally, the input signal can vary 
     * between [-1,1]. We reduce this interval to avoid the robot high speed.  
     * 
     */
    class CMotorDriverVnh:public ICurrentGetter, public IMotorCommand
    {
    public:
        /* Constructor */
        CMotorDriverVnh(PinName, PinName, PinName, PinName);
        /* Constructor */
        CMotorDriverVnh(PinName, PinName, PinName, PinName,float,float);
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
        /* Check the allowed range */
        bool inRange(float f_pwm);
        
    private:
        /** @brief PWM output pin */
        PwmOut      m_pwm;
        /** @brief pin A for direction */
        DigitalOut  m_ina;
        /** @brief pin B for direction */
        DigitalOut  m_inb;
        /** @brief Measured current value by driver */
        AnalogIn    m_current_in;

        const float m_inf_limit;
        const float m_sup_limit;
    };


}; // namespace hardware::drivers


#endif
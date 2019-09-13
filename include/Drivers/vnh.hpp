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
#ifndef VNH_HPP
#define VNH_HPP

/* The mbed library */
#include <mbed.h>
/* Functions to compute common mathematical operations and transformations */
#include <cmath>

namespace drivers{
    // ! Current getter interface
    class ICurrentGetter{
        public:
            virtual float GetCurrent(void)=0;
    };

    /**
     * @brief Command setter interface.
     * 
     */
    class ICommandSetter{
        public:
            virtual void Run(float pwm);
    };

    //!  VNH class. 
    /*! 
    *  It is used for implementing motor bridge control.
    * */
    class VNH:public ICurrentGetter, public ICommandSetter
    {
    public:
        /* Constructor */
        VNH(PinName, PinName, PinName, PinName);
        /* Destructor */
        ~VNH();
        /* Start */
        void Start(void);
        /* Stop */
        void Stop(void);
        /* Run */
        void Run(float speed);
        /* Brake */
        void Brake();
        /* Inverse */
        void Inverse(float f_speed);
        /* Get current */
        float GetCurrent(void);
        
    private:
        /* Go current */
        void Go(float speed);
        /* increment member */
        float increment;
        /* current speed member */
        float current_speed;
        /* PWM */
        PwmOut      pwm;
        /* pin A */
        DigitalOut  ina;
        /* pin B */
        DigitalOut  inb;
        /* current value */
        AnalogIn    current;
    };


}; // namespace drivers


#endif
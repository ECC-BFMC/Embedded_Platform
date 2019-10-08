/**
  ******************************************************************************
  * @file    SERVO.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-2017
  * @brief   This file contains the class declaration for the steering SERVO
  *          functionality.
  ******************************************************************************
 */

/* Include guard */
#ifndef STEERINGMOTOR_HPP
#define STEERINGMOTOR_HPP

#include <mbed.h>


namespace drivers{

    /**
     * @brief Interface to control the steering angle
     * 
     */
    class ISteeringCommand{
        public:
            virtual void setAngle(float f_angle) = 0 ;
            virtual bool inRange(float f_angle) = 0 ;
    };


    /**
     * @brief It is used to control the servo motor, which is connected to the steering wheels. The steering angle can be accessed through 'setAngle' method. 
     * 
     */
    class CSteeringMotor: public ISteeringCommand
    {
    public:
        /* Constructor */
        CSteeringMotor(PinName f_pwm);
        /* Constructor */
        CSteeringMotor(PinName f_pwm,float f_inf_limit,float f_sup_limit);
        /* Destructor */
        ~CSteeringMotor();
        /* Set angle */
        void setAngle(float f_angle); 
        bool inRange(float f_angle);
    private:
        /* convert angle degree to duty cycle for pwm signal */
        float conversion(float f_angle); //angle to duty cycle
        /* PWM output pin */
        PwmOut m_pwm;

        /* Range */
        const float m_inf_limit;
        const float m_sup_limit;
    };
}; // namespace drivers


#endif
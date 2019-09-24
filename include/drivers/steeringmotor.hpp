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
#ifndef SERVO_HPP
#define SERVO_HPP

#include <mbed.h>


namespace drivers{
    /**
     * @brief It is used to control the servo motor, which is connected to the steering wheels. The steering angle can be accessed through 'setAngle' method. 
     * 
     */
    class CSteeringMotor 
    {
    public:
        /* Constructor */
        CSteeringMotor(PinName _pwm);
        /* Destructor */
        ~CSteeringMotor();
        /* Set angle */
        void setAngle(float angle); //-25 to 25 degr
    private:
        /* convert angle degree to duty cycle for pwm signal */
        float conversion(float angle); //angle to duty cycle
        /* PWM value */
        PwmOut pwm;
        /* Current angle */
        float current_angle;
    };
}; // namespace drivers


#endif
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
     * @brief It is used for implementing SERVO functionality.
     * 
     */
    class SERVO
    {
    public:
        /* Constructor */
        SERVO(PinName _pwm);
        /* Destructor */
        ~SERVO();
        /* Set angle */
        void SetAngle(float angle); //-25 to 25 degr
    private:
        /* convert angle to duty cycle */
        float conversion(float angle); //angle to duty cycle
        /* PWM value */
        PwmOut pwm;
        /* Current angle */
        float current_angle;
    };
}; // namespace drivers


#endif
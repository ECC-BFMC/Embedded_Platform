/**
Copyright 2019 Bosch Engineering Center Cluj and BFMC organizers 

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
     
  ******************************************************************************
  * @file    steeringmotor.hpp
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


namespace hardware::drivers{

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
     * @brief Steering servo motor driver
     * 
     * It is used to control the servo motor, which is connected to the steering wheels. The steering angle can be accessed through 'setAngle' method. 
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
        /** @brief PWM output pin */
        PwmOut m_pwm;

        /** @brief Inferior limit */
        const float m_inf_limit;
        /** @brief Superior limit */
        const float m_sup_limit;
    };
}; // namespace hardware::drivers


#endif
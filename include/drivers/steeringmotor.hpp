/**
 * Copyright (c) 2019, Bosch Engineering Center Cluj and BFMC organizers
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:

 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.

 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.

 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
*/

/* Include guard */
#ifndef STEERINGMOTOR_HPP
#define STEERINGMOTOR_HPP

#include <mbed.h>
#include <utility>
#include <cmath>
#include <climits>
#include <chrono>

#include <brain/globalsv.hpp>

namespace drivers
{
    /**
     * @brief Interface to control the steering angle
     * 
     */
    class ISteeringCommand
    {
        public:
            virtual void setAngle(int f_angle) = 0 ;
            virtual int inRange(int f_angle) = 0 ;
            virtual int get_upper_limit() = 0 ;
            virtual int get_lower_limit() = 0 ;

            int16_t pwm_value = 0;
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
            CSteeringMotor(
                PinName f_pwm_pin,
                int f_inf_limit,
                int f_sup_limit
            );
            /* Destructor */
            ~CSteeringMotor();
            /* Set angle */
            void setAngle(int f_angle); 
            /* Check if angle in range */
            int inRange(int f_angle);
            int get_upper_limit();
            int get_lower_limit();
        private:
            /** @brief PWM output pin */
            PwmOut m_pwm_pin;
            /** @brief 0 default */
            int zero_default = 1500; //0.075(7.5% duty cycle) * 20000µs(ms_period)
            /** @brief ms_period */
            int8_t ms_period = 20; // 20000µs
            
            /** @brief Inferior limit */
            const int m_inf_limit;
            /** @brief Superior limit */
            const int m_sup_limit;

            /* convert angle degree to duty cycle for pwm signal */
            int computePWMPolynomial(int steering); //angle to duty cycle

            /* interpolate the pwm value and the zero default based on the steering value */
            int16_t interpolate(int steering, const int steeringValueP[], const int steeringValueN[], const int pwmValuesP[], const int pwmValuesN[], int size);

            // Predefined values for steering reference and interpolation
            const int steeringValueP[3] = {0, 150, 200};
            const int steeringValueN[3] = {0, -150, -200};

            const int pwmValuesP[3] = {
                1500, 1801, 1914
            };

            const int pwmValuesN[3] = {
                1500, 1285, 1154
            };

    }; // class ISteeringCommand
}; // namespace drivers


#endif //STEERINGMOTOR_HPP
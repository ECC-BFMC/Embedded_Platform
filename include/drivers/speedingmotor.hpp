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
#ifndef SPEEDINGMOTOR_HPP
#define SPEEDINGMOTOR_HPP

/* The mbed library */
#include <mbed.h>

namespace drivers
{
    /**
     * @brief Interface to control the brushless motor.
     * 
     */
    class ISpeedingCommand
    {
        public:
            virtual void setSpeed(int f_speed) = 0 ;
            virtual bool inRange(int f_speed) = 0 ;
            virtual void setBrake() = 0 ;
    };

    /**  
     * @brief Speeding motor driver
     * 
     * It is used to control the Brushless motor (more precisely the ESC), which is connected to driving shaft. The reference speed can be accessed through 'setSpeed' method. 
     * 
     */
    class CSpeedingMotor: public ISpeedingCommand
    {
        public:
            /* Constructor */
            CSpeedingMotor(
                PinName     f_pwm_pin,
                int       f_inf_limit,
                int       f_sup_limit
            );
            /* Destructor */
            ~CSpeedingMotor();
            /* Set speed */
            void setSpeed(int f_speed); 
            /* Check speed is in range */
            bool inRange(int f_speed);
            /* Set brake */
            void setBrake(); 

        private:
            /** @brief PWM output pin */
            PwmOut m_pwm_pin;
            /** @brief 0 default */
            uint16_t zero_default = 1491; //0.074568(7.4% duty cycle) * 20000µs(ms_period)
            /** @brief 0 default */
            uint8_t ms_period = 20; // 20000µs
            /** @brief step_value */
            int16_t step_value = 102;  // 0.00051 * 20000µs(ms_period) * 10(scale factor)
            /** @brief Inferior limit */
            const int m_inf_limit;
            /** @brief Superior limit */
            const int m_sup_limit;

            /* interpolate the step value based on the speed value */
            int16_t interpolate(int speed, const int speedValuesP[], const int speedValuesN[], const int stepValues[], int size);

            // Scaled predefined values for speeding reference and interpolation
            const int speedValuesP[25] = {
                 40, 50, 60, 70, 80, 90, 100, 110, 120, 130,
                140, 150, 160, 170, 180, 190, 200, 210, 220, 260,
                300, 350, 400, 450, 500
            };

            const int speedValuesN[25] = {
                 -40, -50, -60, -70, -80, -90, -100, -110, -120, -130,
                -140, -150, -160, -170, -180, -190, -200, -210, -220, -260,
                -300, -350, -400, -450, -500
            };

            // StepValues have a scale factor applied (*10)
            const int stepValues[25] = {
                214, 176, 152, 134, 120, 110, 102, 94, 86, 82,
                78, 74, 70, 68, 66, 64, 60, 58, 56, 50,
                48, 42, 38, 36, 34
            };

            int conversion(int f_speed); //angle to duty cycle
    }; // class CSpeedingMotor
}; // namespace drivers

#endif// SPEEDINGMOTOR_HPP
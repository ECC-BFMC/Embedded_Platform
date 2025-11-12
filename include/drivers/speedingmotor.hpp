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
#include <brain/globalsv.hpp>

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
            virtual int inRange(int f_speed) = 0 ;
            virtual void setBrake() = 0 ;
            virtual int get_upper_limit() = 0 ;
            virtual int get_lower_limit() = 0 ;
            
            int16_t pwm_value = 0; 
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
            int inRange(int f_speed);
            /* Set brake */
            void setBrake();
            int get_upper_limit();
            int get_lower_limit();
        private:
            /** @brief PWM output pin */
            PwmOut m_pwm_pin;
            /** @brief 0 default */
            uint16_t zero_default = 1491; //0.074568(7.4% duty cycle) * 20000µs(ms_period)
            /** @brief 0 default */
            uint8_t ms_period = 20; // 20000µs
            
            /** @brief Inferior limit */
            const int m_inf_limit;
            /** @brief Superior limit */
            const int m_sup_limit;

            /* interpolate the pwm value based on the speed value */
            int16_t interpolate(int speed, const int speedValuesP[], const int speedValuesN[], const int pwmValuesP[], const int pwmValuesN[], int size);

            int computePWMPolynomial(int speed); //angle to duty cycle

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

            const int pwmValuesP[25] = {
                1576, 1579, 1582, 1584, 1587, 1590, 1593, 1594, 1594, 1597,
                1600, 1602, 1603, 1606, 1609, 1612, 1611, 1612, 1614, 1621,
                1635, 1638, 1643, 1653, 1661
            };

            const int pwmValuesN[25] = {
                1405, 1403, 1399, 1397, 1395, 1392, 1389, 1387, 1387, 1384,
                1381, 1380, 1379, 1375, 1372, 1369, 1371, 1369, 1367, 1361,
                1347, 1344, 1339, 1329, 1321
            };

    }; // class CSpeedingMotor
}; // namespace drivers

#endif// SPEEDINGMOTOR_HPP
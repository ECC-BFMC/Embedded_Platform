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


/* Inclusion guard */
#ifndef DCMOTOR_HPP
#define DCMOTOR_HPP

/* The mbed library */
#include <mbed.h>
/* Functions to compute common mathematical operations and transformations */
#include <cmath>

namespace hardware::drivers{

    /**
     * @brief Command setter interface.
     * 
     */
    class IMotorCommand{
        public:
            virtual void setSpeed(float f_pwm) = 0;
            virtual void brake() = 0;
            virtual bool inRange(float f_pwm) =0;
    };

    /**
     * @brief VNH5019 Motor Driver class 
     * 
     * It has role to control the vnh5019 motor driver: the motor speed and the direction.
     * The direction and the rotation speed are regulated by a single value, where the positive values define the forward move, 
     * similary negative values represent the backward move. The magnitude of values gives the motor speed. So generally, 
     * the input signal can vary between [-1,1]. We reduced this interval to avoid the robot high speed.  
     * 
     */
    class CMotorDriverVnh:public IMotorCommand {
        public:
            /* Constructor */
            CMotorDriverVnh(PinName, PinName, PinName, float, float);
            /* Destructor */
            ~CMotorDriverVnh();
            /* Run */
            void setSpeed(float f_pwm);
            /* Brake */
            void brake();
            /* Check the allowed range */
            bool inRange(float f_pwm);
            
        private:
            /** @brief PWM output pin */
            PwmOut      m_pwm;
            /** @brief pin A for direction */
            DigitalOut  m_ina;
            /** @brief pin B for direction */
            DigitalOut  m_inb;

            const float m_pwm_inf_limit;
            const float m_pwm_sup_limit;
    };


}; // namespace hardware::drivers


#endif
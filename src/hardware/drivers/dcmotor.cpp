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


#include <hardware/drivers/dcmotor.hpp>


namespace hardware::drivers{

    /**  @brief
     *  It initializes a object parameter based the given input paramterers, like input, output lines and the limits. 
     * The negative value of limit specifies the maximum backward speed, similarly the positive value gives the maximum forward speed.
     *
     *  @param f_pwmPin        PWM pin
     *  @param f_inaPin        pin A
     *  @param f_inbPin        pin B
     *  @param f_currentPin    current analog pin
     *  @param f_inf_limit     inferior limit
     *  @param f_sup_limit     superior limit
     */
    CMotorDriverVnh::CMotorDriverVnh(PinName f_pwmPin, PinName f_inaPin, PinName f_inbPin, float f_pwm_inf_limit, float f_pwm_sup_limit)
        :m_pwm(f_pwmPin)
        ,m_ina(f_inaPin)
        ,m_inb(f_inbPin)
        ,m_pwm_inf_limit(f_pwm_inf_limit)
        ,m_pwm_sup_limit(f_pwm_sup_limit)
    {  
        m_pwm.period_us(200);
    }

    /**  @brief  VNH class destructor
     */
    CMotorDriverVnh::~CMotorDriverVnh()
    {
    }

    /**  @brief This method manipulates the output signals for driver to control the motor rotation. 
     * The positive duty cycle value generates a motor rotation, which causes forward move of the robot. A negative value
     * produces a backward move.  
     *
     *  @param f_pwm     duty cycle of generated pwm signal
     *   
     */
    void CMotorDriverVnh::setSpeed(float f_pwm)
    {
        // Setting direction of the motor based on pwm. Positive pwm refers to forward direction and negative pwm to backward. 
        if (f_pwm < 0) //backward
        {
            m_ina = 0;
            m_inb = 1;
        }
        else //forward
        {
            m_ina = 1;
            m_inb = 0;
        } 
        m_pwm = std::abs(f_pwm);
    }

    /**  @brief  It brakes the motor rotation, working like a dynamic braking method. 
     *   
     */
    void CMotorDriverVnh::brake()
    {
        m_ina = 0;
        m_inb = 0;
        m_pwm.write(0.0);
    }

    /**
     * @brief It checks whether a number is in a given range
     * 
     * @param f_pwm value 
     * @return true means, that the value is in the range
     * @return false means, that the value isn't in the range
     */
    bool CMotorDriverVnh::inRange(float f_pwm){
        return m_pwm_inf_limit<=f_pwm && f_pwm <=m_pwm_sup_limit;
    }

}; // namespace hardware::drivers
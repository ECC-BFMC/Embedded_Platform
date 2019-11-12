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
  * @file    VNH.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-2017
  * @brief   This file contains the class defoinition for the VNH Bridge Driver
  *          functionality.
  ******************************************************************************
 */

#include <hardware/drivers/dcmotor.hpp>


namespace hardware::drivers{



    /**  @brief 
     *  It initializes a object parameter based the given input paramterers, like input and output lines. The limits
     * of the drivers are -0.5 and 0.5 duty cycle of generated PWM signal. 
     *
     *  @param f_pwmPin      PWM pin
     *  @param f_inaPin      pin A
     *  @param f_inbPin      pin B
     *  @param f_currentPin  current analog pin
     */
    CMotorDriverVnh::CMotorDriverVnh(PinName f_pwmPin, PinName f_inaPin, PinName f_inbPin, PinName f_currentPin)
        :m_pwm(f_pwmPin)
        ,m_ina(f_inaPin)
        ,m_inb(f_inbPin)
        ,m_current_in(f_currentPin)
        ,m_inf_limit(-0.50)
        ,m_sup_limit(0.50)
    {  
        m_pwm.period_us(200);
    }


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
    CMotorDriverVnh::CMotorDriverVnh(PinName f_pwmPin, PinName f_inaPin, PinName f_inbPin, PinName f_currentPin, float f_inf_limit, float f_sup_limit)
        :m_pwm(f_pwmPin)
        ,m_ina(f_inaPin)
        ,m_inb(f_inbPin)
        ,m_current_in(f_currentPin)
        ,m_inf_limit(f_inf_limit)
        ,m_sup_limit(f_sup_limit)
    {  
        m_pwm.period_us(200);
    }

    /**  @brief  VNH class destructor
     */
    CMotorDriverVnh::~CMotorDriverVnh()
    {
    }

    /**  @brief This method manipulates the output signals for driver to control the motor rotation. 
     * The positive duty cycle value generates a motor rotation, which causes forward move of robots. A negative value
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

    /**  @brief It returns measured current on the motor driver.
     *   \note Need to be tested. 
     *   
     *  \return    Meausered current value of dc motor [A].
     */ 
    float CMotorDriverVnh::getCurrent(void)
    {
        return (m_current_in.read()) * 5 / 0.14;
    }

    /**  @brief It inverses the rotation direction of the motor and set a new duty cycle value. 
     *
     *  @param f_pwm  duty cycle of generated pwm signal
     *   
     */
    void CMotorDriverVnh::inverseDirection(float f_pwm)
    {
        m_ina=!m_ina;
        m_inb=!m_inb;
        m_pwm =std::abs(f_pwm);
    }

    /**  @brief  It brakes the motor rotation, working like a dynamic braking method. 
     *   
     */
    void CMotorDriverVnh::brake()
    {
        m_ina.write(0);
        m_inb.write(0);
        m_pwm.write(100.0);
    }

    /**
     * @brief It checks whether a number is in a given range
     * 
     * @param f_pwm value 
     * @return true means, that the value is in the range
     * @return false means, that the value isn't in the range
     */
    bool CMotorDriverVnh::inRange(float f_pwm){
        return m_inf_limit<=f_pwm && f_pwm <=m_sup_limit;
    }

}; // namespace hardware::drivers
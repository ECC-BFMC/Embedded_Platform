/**
  ******************************************************************************
  * @file    VNH.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-2017
  * @brief   This file contains the class defoinition for the VNH Bridge Driver
  *          functionality.
  ******************************************************************************
 */

#include <drivers/dcmotor.hpp>


namespace drivers{



    /**  @brief  VNH class constructor
     *
     *  Constructor method
     *
     *  @param _pwm      PWM pin
     *  @param _ina      pin A
     *  @param _inb      pin B
     *  @param _current  current analog pin
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


    /**  @brief  VNH class constructor
     *
     *  Constructor method
     *
     *  @param _pwm      PWM pin
     *  @param _ina      pin A
     *  @param _inb      pin B
     *  @param _current  current analog pin
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
     *
     *  Destructor method
     *
     *   
     */
    CMotorDriverVnh::~CMotorDriverVnh()
    {
    }

    /**  @brief  Run method
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

    /**  @brief  Get measured current on the motor driver.
     *   \note Need to be tested. 
     *   
     *  \return    Current value
     */
    float CMotorDriverVnh::getCurrent(void)
    {
        return (m_current_in.read()) * 5 / 0.14;
    }

    /**  @brief  Inverse the rotation direction of the motor and set a new duty cycle value. 
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

    /**  @brief  Brake the motor rotation, working like a dynamic braking method. 
     *   
     */
    void CMotorDriverVnh::brake()
    {
        m_ina.write(0);
        m_inb.write(0);
        m_pwm.write(100.0);
    }

    /**
     * @brief check whether a number is in a given range
     * 
     * @param f_pwm value 
     * @return true is in the range
     * @return false isn't in the range
     */
    bool CMotorDriverVnh::inRange(float f_pwm){
        return m_inf_limit<=f_pwm && f_pwm <=m_sup_limit;
    }

}; // namespace drivers
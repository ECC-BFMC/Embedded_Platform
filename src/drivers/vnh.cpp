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

#include <Drivers/vnh.hpp>


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
    VNH::VNH(PinName _pwm, PinName _ina, PinName _inb, PinName _current)
        :pwm(_pwm)
        ,ina(_ina)
        ,inb(_inb)
        ,current(_current)
    {  
        pwm.period_us(200);   
        increment = 0.01;
        current_speed = 0;
    }

    /**  @brief  VNH class destructor
     *
     *  Destructor method
     *
     *   
     */
    VNH::~VNH()
    {
    }

    /**  @brief  Start method
     *
     *   
     *   
     */
    void VNH::Start(void)
    {
        ina = !inb;
        pwm.write(0);
    }

    /**  @brief  Stop method
     *
     *   
     *   
     */
    void VNH::Stop(void)
    {
        ina = inb;
    }

    /**  @brief  Run method
     *
     *  @param speed    speed value
     *   
     */
    void VNH::Run(float speed)
    {
        Go(speed);
    }

    /**  @brief  Get current method
     *
     *   
     *  \return    Current value
     */
    float VNH::GetCurrent(void)
    {
        return (current.read()) * 5 / 0.14;
    }

    /**  @brief  Go method
     *
     *  @param speed    speed value
     *   
     */
    void VNH::Go(float speed)
    {
        //pwm.write(abs(speed));
        if (speed < 0)
        {
            ina = 0;
            inb = 1;
        }
        else
        {
            ina = 1;
            inb = 0;
        } 
        pwm =std::abs(speed);
    }

    /**  @brief  Inverse method
     *
     *  @param f_speed  speed value
     *   
     */
    void VNH::Inverse(float f_speed)
    {
        ina=!ina;
        inb=!inb;
        pwm =std::abs(f_speed);
    }

    /**  @brief  Brake method
     *
     *   
     *   
     */
    void VNH::Brake()
    {
        ina.write(0);
        inb.write(0);
        pwm.write(100.0);
    }

}; // namespace drivers
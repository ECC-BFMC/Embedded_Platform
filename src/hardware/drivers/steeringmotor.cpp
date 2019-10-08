/**
  ******************************************************************************
  * @file    SERVO.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-2017
  * @brief   This file contains the class definition for the steering SERVO
  *          functionality.
  ******************************************************************************
 */

#include <hardware/drivers/steeringmotor.hpp>


namespace drivers{
    /** @brief  CSteeringMotor class constructor, it  initializes the pwm parameters and set the steering in zero position.
     *
     *  @param _pwm               pin connected to servo motor
     */
    CSteeringMotor::CSteeringMotor(PinName f_pwm)
        :CSteeringMotor(f_pwm,-23.0,23.0)
        
    {
    };

    CSteeringMotor::CSteeringMotor(PinName f_pwm, float f_inf_limit, float f_sup_limit)
        :m_pwm(f_pwm)
        ,m_inf_limit(f_inf_limit)
        ,m_sup_limit(f_sup_limit)
    {
        m_pwm.period_ms(20); 
        // Set position to zero   
        m_pwm.write(0.07525);
    };


    /** @brief  CSteeringMotor class destructor
     *
     *  Destructor method
     */
    CSteeringMotor::~CSteeringMotor()
    {
    };

    /** @brief  Set angle of the servo motor, which controls the steering wheels. 
     *
     *  @param angle      angle degree, where the positive value means right direction and negative value the left direction. 
     */
    void CSteeringMotor::setAngle(float f_angle)
    {
        m_pwm.write(conversion(f_angle));
    };

    /** @brief  Conversion method between angle and pwm signal
     *
     *  It converts angle degree to duty cycle for pwm signal. 
     * 
     *  @param angle    degree
     *  \return         duty cycle in interval [0,1]
     */
    float CSteeringMotor::conversion(float f_angle)
    {
        return (0.0009505 * f_angle + 0.07525);
    };

    /**
     * @brief check whether a number is in a given range
     * 
     * @param f_angle value 
     * @return true is in the range
     * @return false isn't in the range
     */
    bool CSteeringMotor::inRange(float f_angle){
        return m_inf_limit<=f_angle && f_angle<=m_sup_limit;
    };
}; // namespace drivers
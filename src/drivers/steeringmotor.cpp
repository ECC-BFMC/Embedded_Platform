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

#include <drivers/steeringmotor.hpp>


namespace drivers{
    /** @brief  CSteeringMotor class constructor, it  initializes the pwm parameters and set the steering in zero position.
     *
     *  @param _pwm               pin connected to servo motor
     */
    CSteeringMotor::CSteeringMotor(PinName f_pwm)
        :m_pwm(f_pwm)
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
    float CSteeringMotor::conversion(float angle)
    {
        return (0.0009505 * angle + 0.07525);
    };
}; // namespace drivers
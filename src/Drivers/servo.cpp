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

#include <Drivers/servo.hpp>


namespace drivers{
    /** @brief  SERVO class constructor
     *
     *  Constructor method
     *
     *  @param _pwm               pin connected to servo input
     */
    SERVO::SERVO(PinName _pwm)
        :pwm(_pwm)
    {
        pwm.period_ms(20);    
        // pwm.write(0.075);
        pwm.write(0.07525);
    };

    /** @brief  SERVO class destructor
     *
     *  Destructor method
     *
     *   
     */
    SERVO::~SERVO()
    {
    };

    /** @brief  Set angle method
     *
     *  @param angle      angle
     */
    void SERVO::SetAngle(float angle)
    {
        pwm.write(conversion(angle));
    };

    /** @brief  Conversion method
     *
     *  @param angle      angle
     *  \return    duty cycle
     */
    float SERVO::conversion(float angle)
    {
        //rc-servo-4519
        //return (0.0010321101 * angle + 0.0725);
        //fs-5106B
        // return (0.035 +0.0272+ (angle + 25.f) / 1953.135);
        //RS2 MG/BB
        return (0.0009505 * angle + 0.07525);
    };
}; // namespace drivers
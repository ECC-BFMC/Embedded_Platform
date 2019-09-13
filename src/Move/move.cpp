/**
  ******************************************************************************
  * @file    Move.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class definition for the moving
  *          functionality.
  ******************************************************************************
 */

#include "mbed.h"
#include <Move/move.hpp>
//#include <array.h>

/** \brief  Move Class constructor
 *
 *  Constructor method
 *
 *  @param _pwm_servo
 *  @param _pwm_driver
 *  @param _ina_driver
 *  @param _inb_driver
 *  @param _current_driver
 */
Move::Move(
        PinName _pwm_servo,
        PinName _pwm_driver, 
        PinName _ina_driver, 
        PinName _inb_driver, 
        PinName _current_driver)
    :servo(_pwm_servo),
    vnh(_pwm_driver,_ina_driver,_inb_driver,_current_driver)
{
};

/** \brief  Move Class destructor
 *
 *  Destructor method
 *
 *  
 */
Move::~Move()
{
};

/** \brief  Steer method
 *
 *  @param angle
 *  
 */
void Move::Steer(float angle)
{
    if ((angle <= 23) && (angle >= -23))
        servo.SetAngle(angle);
};

/** \brief  Move method
 *
 *  @param speed The Pwm value, must belong to [0,100].
 *  
 */
void Move::Speed(float speed)
{
    speed /=100;
    vnh.Run(speed);
};

/** \brief  Brake method
 *
 *  
 *  
 */
void Move::Brake()
{
    vnh.Brake();
}

/** \brief  Hard brake method
 *
 *  @param f_speed The Pwm value, must belong to [0,100].
 *  
 */
void Move::Inverse(float f_speed)
{
    f_speed /=100;
    vnh.Inverse(f_speed);
}

/** \brief  Car testing method
 *
 *  
 *  
 */
void Move::TestCar()
{
    Steer(20);
    wait(1);
    Steer(0);
    wait(1);
    Steer(-20);
    wait(1);
    ResetCar();
    wait(1);
    Speed(25);
    wait(1);
    Speed(0);
    wait(1);
    Speed(-25);
    wait(1);
    ResetCar();
    wait(1);
};

/** \brief  Reset speed and steer method
 *
 *  
 *  
 */
void Move::ResetCar()
{
    Steer(0);
    Speed(0);
};

/** \brief  Get bridge (driver) object reference method
 *
 *  
 *  @return    reference to VNH object
 */
drivers::VNH& Move::getVNH() 
{ 
    return vnh; 
}
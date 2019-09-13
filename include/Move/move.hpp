/**
  ******************************************************************************
  * @file    MOVE.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class declaration for the moving
  *          functionality.
  ******************************************************************************
 */

/* Include guard */
#ifndef MOVE_HPP
#define MOVE_HPP

#include <Drivers/servo.hpp>
#include <Drivers/vnh.hpp>

/**
 * @brief  Move class
 * It is used for executing move commands.
 * 
 */
class Move
{
public:
    /* Constructor */
    Move(PinName, PinName, PinName, PinName, PinName);//A0
    /* Destructor */
    ~Move();
    /* Steer */
    void Steer(float angle);// -25 to + 25 degrees, - (left), + (right)
    /* Speed */
    void Speed(float speed);//-100 to + 100 -(back), + (front)   
    /* Brake */ 
    void Brake();
    /* Inverse */
    void Inverse(float f_speed);
    /* Test */
    void TestCar();
    /* Reset */
    void ResetCar();
    /* Return bridge object */
    drivers::VNH& getVNH();
            
private:
    /* Servo pbject */
    drivers::SERVO       servo;
    /* Bridge object */
    drivers::VNH         vnh;
};

#endif
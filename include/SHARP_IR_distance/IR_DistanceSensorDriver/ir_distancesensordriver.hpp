/**
  ******************************************************************************
  * @file    IR_DistanceSensorDriver.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    08-January-2018
  * @brief   This file contains the class definition for the SHARP IR sensor read
  *          methods (driver).
  ******************************************************************************
 */

/* Include guard */
#ifndef IR_DISTANCESENSORDRIVER_HPP
#define IR_DISTANCESENSORDRIVER_HPP

#include <mbed.h>
#include <math.h>

/* Module defines */
#define C_IR_DistanceSensorDriver_A -19057.99103709
#define C_IR_DistanceSensorDriver_B 42562.72078701
#define C_IR_DistanceSensorDriver_C -37447.6482552
#define C_IR_DistanceSensorDriver_D 16482.62217927
#define C_IR_DistanceSensorDriver_E -3758.54454044
#define C_IR_DistanceSensorDriver_F 395.72452643

/* //scaling values, variant 1
a=733.06912872;
b=-2221.23062859;
c=2502.20535578;
d=-1273.63067295;
e=269.64209866;
*/

/*
//scaling values, variant 2, used for obtaining more accurate values 
a=-6144.18274059;
b=19775.78355498;
c=-25056.00451678;
d=15739.05258272;
e=-4986.65539646;
f=673.27540402;
*/
/*scaling values, variant 3 (media benzii)
a=-19057.99103709;
b=42562.72078701;
c=-37447.6482552;
d=16482.62217927;
e=-3758.54454044;
f=395.72452643;
*/

/**
 * @brief It is used for reading SHARP IR distance sensors.
 * 
 */
class C_IR_DistanceSensorDriver
{
public:
    /* Construnctor */
    C_IR_DistanceSensorDriver(AnalogIn ain_pin, DigitalOut en_pin);
    /* Destrunctor */
    ~C_IR_DistanceSensorDriver();
    /* Method for reading distance */
    float ReadDistance(void);
    /* Method for enabling sensor */
    void enable(void);
    /* Method for disabling sensor */
    void disable(void);
    //void setValue(float);
    /* Method for obtaining read value */ 
    float getValue(void);

private:

    /* the analog pin, giving the proportional voltage value */
    AnalogIn ain_pin;
    /* enable pin */
    DigitalOut en_pin;
    /* distance value */
    float value;
};

/* Include guard */
#endif
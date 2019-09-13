/**
  ******************************************************************************
  * @file    DistanceSensors.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    08-January-2018
  * @brief   This file contains the class definition for the distance sensors read
  *          methods.
  ******************************************************************************
 */

/* Include guard */
#ifndef DISTANCESENSORS_HPP
#define DISTANCESENSORS_HPP

#include <mbed.h>
#include <array>
#include <string>
#include<TaskManager/taskmanager.hpp>
#include <SHARP_IR_distance\DistanceSensors\distancesensors.hpp>
using namespace std;

/**
 * @brief It is used for reading the attached distance sensors. It's periodically read all attached sensors measurements, 
 * in one cycle it active and read sequentially all sensors in order to avoid the interference between the sensors. 
 * In the function run it starts the activate the first sensors and the function ReadCallback capture the measurement 
 * and active the next sensor. 
 * 
 */
template<class T>
class CDistanceSensors: public task::CTask
{
private:
    /* The sensors list */
    T                               m_sensor_list;
    /* Index of current sensor */
    uint8_t                         m_index;
    /* The maximum number of sensors that are used */
    const uint8_t                   m_N;
    /* Flags for enabloing/disabling timeout */
    Timeout                         m_disableTimeOut;
    Timeout                         m_enableTimeOut;
    /* Message that contains the read values */
    string                          m_message;

public:
    /* Construnctor */
    CDistanceSensors(uint32_t f_period,
                     float f_baseTick,
                     T&  f_sensor_list);
    /* Destructor */
    ~CDistanceSensors();
    /* Callback for reading sensor value */
    void ReadCallback(void);
    /* Run method */
    void _run(void);
};

#include "DistanceSensors.inl"

/* Include guard */
#endif
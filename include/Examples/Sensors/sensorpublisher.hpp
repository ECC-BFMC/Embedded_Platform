/**
  ******************************************************************************
  * @file    SensorPublisher.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class declaration for the sensor publisher
  *          functionality. Because template classes are used, the code was
  *          placed in a corresponding .inl file and not in a .cpp one.
  ******************************************************************************
 */

/* Include guard */
#ifndef SENSOR_PUBLISHER_HPP
#define SENSOR_PUBLISHER_HPP

/* The mbed library */
#include <mbed.h>
/* Container that encapsulates fixed size arrays */
#include <array>
/* String types, character traits and a set of converting functions */
#include <string>
#include<TaskManager/taskmanager.hpp>


namespace examples{
    namespace sensors
    {
        //!  ProximityPublisher class. 
        /*!
        * It inherits class task::CTask. 
        * It is used for publishing proximity sensor values.
        */
        template <class C_Sensor,uint Nr_Senrsor> 
        class ProximityPublisher:public task::CTask
        {
            public:
                /* Sensor array */
                using SensorArrayT = std::array<C_Sensor*,Nr_Senrsor>;
                /* Constructor */
                ProximityPublisher(uint32_t,SensorArrayT,Serial&);
                /* Serial callback attached to object */
                static void staticSerialCallback(void* obj,char const * a, char * b);
            protected:
                /* Run method */
                void _run();
                /* Serial callback */
                void serialCallback(char const * a, char * b);
            private:
                /* Sensor array */
                SensorArrayT            m_sensors;
                /* Reference to serial object */
                Serial&                 m_serial;
                /* Active flag */
                bool                    m_isActivate;
        };
    }; // namespace sensors
}; // namespace examples

#include "sensorpublisher.inl"
#endif
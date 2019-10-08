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
#ifndef PROXIMITY_PUBLISHER_HPP
#define PROXIMITY_PUBLISHER_HPP

/* The mbed library */
#include <mbed.h>
/* Container that encapsulates fixed size arrays */
#include <array>
/* String types, character traits and a set of converting functions */
#include <string>
#include<utils/taskmanager/taskmanager.hpp>


namespace examples{
    namespace sensors
    {

       /**
        * @brief ProximityPublisher class a periodically task for publishing proximity sensor values.
        * The object of sensors has to contain a 'getValue' method to access the measured values. The publisher
        * can send the values of a multiple sensors based the given array of sensors. 
        * 
        * @tparam C_Sensor      Type of proximity sensors
        * @tparam Nr_Senrsor    Number of sensors 
        */
        template <class C_Sensor,uint Nr_Senrsor> 
        class ProximityPublisher:public task::CTask
        {
            public:
                /* Sensor array */
                using SensorArrayT = std::array<C_Sensor*,Nr_Senrsor>;
                /* Constructor */
                ProximityPublisher(uint32_t,SensorArrayT,Serial&);
                /* Serial callback */
                void serialCallback(char const * a, char * b);
            protected:
                /* Run method */
                void _run();
                
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

#include "proximitypublisher.tpp"
#endif //PROXIMITY_PUBLISHER_HPP
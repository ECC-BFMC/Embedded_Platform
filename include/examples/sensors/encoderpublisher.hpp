/**
  ******************************************************************************
  * @file    SensorTask.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class declaration for the sensor task
  *          functionality.
  ******************************************************************************
 */

/* Include guard */
#ifndef SENSOR_TASK_HPP
#define SENSOR_TASK_HPP

/* Standard C library for basic mathematical operations */
#include <math.h>
#include<utils/taskmanager/taskmanager.hpp>
#include <hardware/drivers/dcmotor.hpp>
#include <utils/serial/serialmonitor.hpp>
#include <signal/controllers/motorcontroller.hpp>
#include <signal/systemmodels/systemmodels.hpp>

#include <hardware/encoders/encoderinterfaces.hpp>

namespace examples
{

    namespace sensors{

       /**
        * @brief CEncoderPublisher class is subclass of utils::task::CTask, a class to publish the encoder values periodically. 
        * 
        */
        class CEncoderPublisher:public utils::task::CTask
        {
            public:
                /* Constructor */
                CEncoderPublisher(uint32_t            f_period
                            ,hardware::encoders::IEncoderGetter&    f_encoder
                            ,Serial&            f_serial);
                /* Serial callback implementation */
                void serialCallback(char const * a, char * b);
            private:
                
                /* Run method */
                void _run();

                /* Active flag */
                bool                m_isActive;
                /*  Encoder value getter */
                hardware::encoders::IEncoderGetter&     m_encoder;
                /* Serial communication member*/
                Serial&             m_serial;
        };
    }; // namespace sensors
}; // namespace examples


#endif
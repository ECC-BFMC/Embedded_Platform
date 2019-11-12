/**
Copyright 2019 Bosch Engineering Center Cluj and BFMC organizers 

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

  ******************************************************************************
  * @file    encoderpublisher.hpp
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
        * @brief CEncoderPublisher class is subclass of utils::task::CTask, a class to publish periodically the encoder values. 
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

                /** @brief Active flag  */
                bool                m_isActive;
                /** @brief Encoder getter interface  */
                hardware::encoders::IEncoderGetter&     m_encoder;
                /** @brief Serial communication obj.  */
                Serial&             m_serial;
        };
    }; // namespace sensors
}; // namespace examples


#endif
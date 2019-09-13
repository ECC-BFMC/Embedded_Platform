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
#include<TaskManager/taskmanager.hpp>
#include <Drivers/vnh.hpp>
#include <SerialMonitor/serialmonitor.hpp>
#include <Controllers/controller.hpp>
#include <SystemModels/systemmodels.hpp>

#include <Encoders/encoderinterface.hpp>

namespace examples
{

    namespace sensors{

    
        //!  CDISTFTest class. 
        /*!
        * It inherits class task::CTask. 
        * It is used for testing distance sensors operation.
        */
        class CDISTFTest:public task::CTask
        {
            public:
                /* Constructor */
                CDISTFTest(uint32_t        f_period
                        ,Serial&         f_serial);
                
            private:
                /* Run method */
                void _run();
                
                /* Reference to serial object */
                Serial&                                                       m_serial;
                /* Transfer function */
                systemmodels::lti::siso::CDiscreteTransferFucntion<float,3,3> m_tf;
        };

        //!  CEncoderSender class. 
        /*!
        * It inherits class task::CTask. 
        * It is used for sending acquired data.
        */
        class CEncoderSender:public task::CTask
        {
            public:
                /* Constructor */
                CEncoderSender(uint32_t            f_period
                            ,encoders::IEncoderGetter&    f_encoder
                            ,Serial&            f_serial);
                /* Serial callback */
                static void staticSerialCallback(void* obj,char const * a, char * b);
            private:
                /* Serial callback implementation */
                void serialCallback(char const * a, char * b);
                /* Run method */
                void _run();

                /* Active flag */
                bool                m_isActived;
                /*  Encoder value getter */
                encoders::IEncoderGetter&     m_encoder;
                /* Serial communication member*/
                Serial&             m_serial;
        };
    }; // namespace sensors
}; // namespace examples


#endif
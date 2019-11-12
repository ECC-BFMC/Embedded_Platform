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
  * @file    SensorTask.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class implementation for the sensor task
  *          functionality.
  ******************************************************************************
 */
#include <examples/sensors/encoderpublisher.hpp>

namespace examples
{

    namespace sensors{

        /** \brief CEncoderPublisher contructor
         *
         *  It initializes the member parameter, the publisher is initially deactivated.
         *
         *  @param f_period       period value
         *  @param f_encoder      reference to encoder object
         *  @param f_serial       reference to the serial object
         */
        CEncoderPublisher::CEncoderPublisher(uint32_t            f_period
                                                ,hardware::encoders::IEncoderGetter&     f_encoder
                                                ,Serial&             f_serial)
            :utils::task::CTask(f_period)
            ,m_isActive(false)
            ,m_encoder(f_encoder)
            ,m_serial(f_serial)
        {
        }

       
        /** \brief  Serial callback method to activate or deactivate the publisher. 
         * When the received integer value is bigger or equal to 1, then the publisher become 
         * active and send messages, otherwise is deactivated. 
         *
         * @param a                   input received string
         * @param b                   output reponse message
         * 
         */
        void CEncoderPublisher::serialCallback(char const * a, char * b){
            int l_isActivate=0;
            uint32_t l_res = sscanf(a,"%d",&l_isActivate);
            if(l_res==1){
                m_isActive=(l_isActivate>=1);
                sprintf(b,"ack;;");
            }else{
                sprintf(b,"sintax error;;");
            }
        }

        /** \brief It's periodically applied method to send message to other device. 
         */
        void CEncoderPublisher::_run()
        {
            if(!m_isActive) return;
            float l_rps=m_encoder.getSpeedRps();
            m_serial.printf("@ENPB:%.2f;;\r\n",l_rps);  
        }                        

    }; // namespace sensors
}; // namespace examples

/**
 * Copyright (c) 2019, Bosch Engineering Center Cluj and BFMC organizers
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:

 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.

 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.

 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
*/

/* Include guard */
#ifndef SENSOR_TASK_HPP
#define SENSOR_TASK_HPP

/* Standard C library for basic mathematical operations */
#include <math.h>
#include <utils/taskmanager/taskmanager.hpp>
#include <hardware/drivers/dcmotor.hpp>
#include <utils/serial/serialmonitor.hpp>
#include <signal/controllers/motorcontroller.hpp>
#include <signal/systemmodels/systemmodels.hpp>

#include <hardware/encoders/encoderinterfaces.hpp>

namespace periodics
{
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
                        ,RawSerial&            f_serial);
            /* Serial callback implementation */
            void serialCallbackENCODERPUBcommand(char const * a, char * b);
        private:
            
            /* Run method */
            void _run();

            /** @brief Active flag  */
            bool                m_isActive;
            /** @brief Encoder getter interface  */
            hardware::encoders::IEncoderGetter&     m_encoder;
            /** @brief Serial communication obj.  */
            RawSerial&             m_serial;
    };
}; // namespace periodics


#endif
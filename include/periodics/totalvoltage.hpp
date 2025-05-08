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
#ifndef TOTALVOLTAGE_HPP
#define TOTALVOLTAGE_HPP

/* The mbed library */
#include <mbed.h>
/* Header file for the task manager library, which  applies periodically the fun function of it's children*/
#include <utils/task.hpp>
#include <brain/globalsv.hpp>
#include <chrono>

namespace periodics
{
   /**
    * @brief It is used for toggling a LED.
    * 
    */
    class CTotalVoltage : public utils::CTask
    {
        public:
            /* Construnctor */
            CTotalVoltage(
                std::chrono::milliseconds f_period,
                mbed::AnalogIn f_pin, 
                UnbufferedSerial& f_serial
            );
            /* Destructor */
            ~CTotalVoltage();
            /* Serial callback implementation */
            void serialCallbackTOTALVcommand(char const * a, char * b);

            void void_TotalSafetyMeasure();
        private:
            /* Run method */
            virtual void        _run();
            /* ADC input pin for instand consume */
            mbed::AnalogIn      m_pin;  
            /* @brief Serial communication obj.  */
            UnbufferedSerial&          m_serial;
            /** @brief Active flag  */
            bool                m_isActive;
            /* @brief ADC counter */
            uint8_t            m_adcCounter;
            
    }; // class CTotalVoltage
}; // namespace periodics

#endif // TOTALVOLTAGE_HPP
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

#ifndef POWERMANAGER_HPP
#define POWERMANAGER_HPP

// TODO: Add your code here
#include <mbed.h>
/* Header file for the task manager library, which  applies periodically the fun function of it's children*/
#include <utils/task.hpp>
#include <brain/globalsv.hpp>
#include <brain/klmanager.hpp>
#include <periodics/totalvoltage.hpp>
#include <periodics/instantconsumption.hpp>
#include <periodics/alerts.hpp>
#include <chrono>

namespace periodics
{
   /**
    * @brief Class powermanager
    *
    */
    class CPowermanager: public utils::CTask
    {
        public:
            /* Construnctor */
            CPowermanager(
                std::chrono::milliseconds f_period,
                brain::CKlmanager& f_CKlmanager,
                UnbufferedSerial& f_serial,
                periodics::CTotalVoltage& f_totalVoltage,
                periodics::CInstantConsumption& f_instantConsumption,
                periodics::CAlerts& f_alerts
            );
            /* Destructor */
            ~CPowermanager();
        private:
            /* private variables & method member */
            
            /* Run method */
            virtual void        _run();

            brain::CKlmanager& m_CKlmanager;

            UnbufferedSerial& m_serial;

            periodics::CTotalVoltage& m_totalVoltage;

            periodics::CInstantConsumption& m_instantConsumption;

            periodics::CAlerts& m_alerts;

            uint16_t m_period;

            uint8_t m_warningCounter;
            uint8_t m_shutdownCounter;

    }; // class CPowermanager
}; // namespace brain

#endif // POWERMANAGER_HPP

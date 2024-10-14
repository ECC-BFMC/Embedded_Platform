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

#ifndef KLMANAGER_HPP
#define KLMANAGER_HPP

// TODO: Add your code here

/* The mbed library */
#include <mbed.h>
#include <periodics/alerts.hpp>
#include <periodics/imu.hpp>
#include <periodics/instantconsumption.hpp>
#include <periodics/totalvoltage.hpp>
#include <brain/robotstatemachine.hpp>
#include <periodics/resourcemonitor.hpp>
#include <brain/globalsv.hpp>

namespace brain
{
   /**
    * @brief Class klmanager
    *
    */
    class CKlmanager
    {
        public:
            /* Construnctor */
            CKlmanager(
                periodics::CAlerts& f_alerts,
                periodics::CImu& f_imu,
                periodics::CInstantConsumption& f_instant,
                periodics::CTotalVoltage& f_baterry,
                brain::CRobotStateMachine& f_robotStateMachine,
                periodics::CResourcemonitor& f_resourceM
            );
            /* Destructor */
            ~CKlmanager();

            void serialCallbackKLCommand(const char* message, char* response);

            uint8_t m_klvalue;

        private:
            /* private variables & method member */
            periodics::CAlerts& m_alerts;
            periodics::CImu& m_imu;
            periodics::CInstantConsumption& m_instant;
            periodics::CTotalVoltage& m_baterry;
            brain::CRobotStateMachine& m_robotStateMachine;
            periodics::CResourcemonitor& m_resourceM;

    }; // class CKlmanager
}; // namespace brain

#endif // KLMANAGER_HPP

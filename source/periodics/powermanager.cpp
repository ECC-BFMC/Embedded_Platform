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

#include "periodics/powermanager.hpp"

#define seconds_in_h 3600
#define battery_mAmps 6000
#define battery_cells 2
#define battery_maxVoltage (4200 * battery_cells)
#define battery_shutdownVoltage 7000
#define battery_shutdownWarning 7200
#define _22_chars 22
#define _1_char 1
#define scale_factor 1000
#define counter_target 250
#define counter_shutdown 15

// TODO: Add your code here
namespace periodics
{
   /**
    * @brief Class constructorpowermanager
    *
    */
    CPowermanager::CPowermanager(
        std::chrono::milliseconds f_period,
        brain::CKlmanager& f_CKlmanager,
        UnbufferedSerial& f_serial,
        periodics::CTotalVoltage& f_totalVoltage,
        periodics::CInstantConsumption& f_instantConsumption,
        periodics::CAlerts& f_alerts
    )
    : utils::CTask(f_period)
    , m_CKlmanager(f_CKlmanager)
    , m_serial(f_serial)
    , m_totalVoltage(f_totalVoltage)
    , m_instantConsumption(f_instantConsumption)
    , m_alerts(f_alerts)
    , m_period((uint16_t)(f_period.count()))
    , m_warningCounter(0)
    , m_shutdownCounter(0)
    {
        /* constructor behaviour */
    }

    /** @brief  CPowermanager class destructor
     */
    CPowermanager::~CPowermanager()
    {
    };

    void CPowermanager::_run()
    {
        char buffer[_22_chars];
        uint16_t battery_mAmps_var;

        m_totalVoltage.void_TotalSafetyMeasure();
        m_instantConsumption.void_InstantSafetyMeasure(m_period);

        if(uint16_globalsV_battery_mAmps_user != 0) battery_mAmps_var = uint16_globalsV_battery_mAmps_user;
        else battery_mAmps_var = battery_mAmps;

        int mAmps_actual = (uint16_globalsV_battery_totalVoltage * battery_mAmps_var) / battery_maxVoltage;

        uint32_globalsV_range_left_shutdown = (mAmps_actual - (battery_shutdownVoltage*battery_mAmps_var/battery_maxVoltage))*scale_factor;

        uint32_globalsV_range_left_shutdown /= (uint16_t)currentEMA;

        uint32_globalsV_range_left_shutdown *= seconds_in_h;

        uint32_globalsV_range_left_shutdown /= scale_factor;

        bool_globalsV_ShuttedDown = false;

        if((battery_shutdownVoltage < uint16_globalsV_battery_totalVoltage) && (uint16_globalsV_battery_totalVoltage <= battery_shutdownWarning))
        {
            m_warningCounter += 1;
            m_shutdownCounter = 0;

            if((m_warningCounter == counter_target) && (uint32_globalsV_numberOfMiliseconds_Total > 0))
            {
                if(!bool_globalsV_warningFlag)
                {
                    uint8_t h = (uint8_t)(uint32_globalsV_range_left_shutdown/3600);

                    uint8_t m = (uint8_t)((uint32_globalsV_range_left_shutdown%3600)/60);

                    uint8_t s = (uint8_t)((uint32_globalsV_range_left_shutdown%3600)%60);

                    bool_globalsV_warningFlag = !bool_globalsV_warningFlag;

                    snprintf(buffer, sizeof(buffer), "@warning:%hhu:%hhu:%hhu;;\r\n", h, m, s);
                    m_serial.write(buffer, strlen(buffer));

                    m_alerts.alertsCommand("1",buffer);
                }
            }
            else
            {
                bool_globalsV_warningFlag = !bool_globalsV_warningFlag;
            }   
        }
        else if ((uint16_globalsV_battery_totalVoltage <= battery_shutdownVoltage) && (uint16_globalsV_battery_totalVoltage > 0))
        {   
            m_shutdownCounter += 1;

            if(m_shutdownCounter == counter_shutdown)
            {   
                snprintf(buffer, sizeof(buffer), "@shutdown:ack;;\r\n");
                m_serial.write(buffer, strlen(buffer));

                char buffer2[_1_char];

                if(uint8_globalsV_value_of_kl!=0)
                {
                    m_CKlmanager.serialCallbackKLCommand("0",buffer2);
                }
                
                ThisThread::sleep_for(chrono::milliseconds(200));
                bool_globalsV_ShuttedDown = true;
                uint8_globalsV_value_of_kl = 0;
            }
        }
    }

}; // namespace periodics
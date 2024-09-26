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

#include "brain/klmanager.hpp"

// TODO: Add your code here
namespace brain
{
   /**
    * @brief Class constructorklmanager
    *
    */
    CKlmanager::CKlmanager(
        periodics::CImu& f_imu,
        periodics::CInstantConsumption& f_instant,
        periodics::CTotalVoltage& f_baterry,
        drivers::CVelocityControlDuration& f_vcd,
        drivers::CResourcemonitor& f_resourceM
    )
    : m_klvalue(0)
    , m_imu(f_imu)
    , m_instant(f_instant)
    , m_baterry(f_baterry)
    , m_vcd(f_vcd)
    , m_resourceM(f_resourceM)
    {
        /* constructor behaviour */
    }

    /** @brief  CKlmanager class destructor
     */
    CKlmanager::~CKlmanager()
    {
    };

    void CKlmanager::serialCallbackKLCommand(const char* a, char* b)
    {
        uint8_t l_keyValue = 0;

        (void)sscanf(a,"%d",&l_keyValue);

        if(!bool_globalsV_ShuttedDown)
        {
            if(l_keyValue == 15 || l_keyValue == 30 || l_keyValue == 0)
            {
                CKlmanager::m_klvalue = l_keyValue;

                char response[50];

                if(l_keyValue == 0 && (int_globalsV_value_of_kl!=0))
                {
                    sprintf(b,"%d",l_keyValue);
                    m_imu.ImuPublisherCommand("0", response);
                    ThisThread::sleep_for(chrono::milliseconds(50));
                    m_instant.InstantPublisherCommand("0", response);
                    ThisThread::sleep_for(chrono::milliseconds(50));
                    m_baterry.TotalPublisherCommand("0", response);
                    ThisThread::sleep_for(chrono::milliseconds(50));
                    m_vcd.serialCallbackVCDCommand("0.0;0.0;0.2", response);
                    ThisThread::sleep_for(chrono::milliseconds(50));
                    m_resourceM.resourceMonitorPublisherCommand("0", response);
                    int_globalsV_value_of_kl = 0;
                }
                if((l_keyValue == 15 || l_keyValue == 30) && (int_globalsV_value_of_kl != 15 && int_globalsV_value_of_kl != 30)) 
                {
                    sprintf(b,"%d",l_keyValue);
                    int_globalsV_value_of_kl = 15;
                    if(!bool_globalsV_imu_isActive) m_imu.ImuPublisherCommand("1", response);
                    if(!bool_globalsV_instant_isActive) m_instant.InstantPublisherCommand("1", response);
                    if(!bool_globalsV_battery_isActive) m_baterry.TotalPublisherCommand("1", response);
                    if(!bool_globalsV_resource_isActive) m_resourceM.resourceMonitorPublisherCommand("1", response);
                }
                if(l_keyValue == 30){
                    sprintf(b,"%d",l_keyValue);
                    int_globalsV_value_of_kl = 30;
                }
            }
            else{
                sprintf(b,"syntax error");
            }
        }
    }

}; // namespace periodics
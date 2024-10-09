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

#include <periodics/totalvoltage.hpp>

#define one_byte 256
#define ref_A1_value_8925mV 65304
#define ref_A1_voltage_mV 8925

namespace periodics{
    /** \brief  Class constructor
     *
     *  It initializes the task and the state of the led. 
     *
     *  \param f_period       Toggling period of LED
     *  \param f_led          Digital output line to LED
     */
    CTotalVoltage::CTotalVoltage(
            std::chrono::milliseconds f_period, 
            mbed::AnalogIn f_pin, 
            UnbufferedSerial& f_serial) 
        : utils::CTask(f_period)
        , m_pin(f_pin)
        , m_serial(f_serial)
        , m_isActive(false)
    {
    }

    /** @brief  CTotalVoltage class destructor
     */
    CTotalVoltage::~CTotalVoltage()
    {
    };

    /** \brief  Serial callback method to activate or deactivate the publisher. 
     * When the received integer value is bigger or equal to 1, then the publisher become 
     * active and send messages, otherwise is deactivated. 
     *
     * @param a                   input received string
     * @param b                   output reponse message
     * 
     */
    void CTotalVoltage::TotalPublisherCommand(char const * a, char * b) {
        uint8_t l_isActivate=0;
        uint8_t l_res = sscanf(a,"%hhu",&l_isActivate);

        if(l_res==1){
            if(int_globalsV_value_of_kl == 15 || int_globalsV_value_of_kl == 30)
            {
                m_isActive=(l_isActivate>=1);
                bool_globalsV_battery_isActive = (l_isActivate>=1);
                sprintf(b,"1");
            }
            else{
                sprintf(b,"kl 15/30 is required!!");
            }
        }else{
            sprintf(b,"syntax error");
        }
    }

    void CTotalVoltage::void_TotalSafetyMeasure()
    {
        uint64_t l_rps = (m_pin.read_u16()*ref_A1_voltage_mV)/ref_A1_value_8925mV;
        int_globalsV_battery_totalVoltage = (uint16_t)l_rps;
        int_globalsV_battery_totalVoltage = (uint16_t)7201;
    }

    /**
    * @brief Periodically reads the battery voltage from A1 pin and sends the scaled value over a serial connection.
    * 
    * When the function is active, it reads a 16-bit value from the pin, which is connected to a battery.
    * The reading is then scaled to represent the actual battery voltage using the provided scale factor: 
    * When the battery voltage is 8.925V, the average value that pin reads is 65303.93.
    * When the battery voltage is 8925mV, the average value that pin reads is 65304.
    * 
    * After obtaining the scaled battery voltage, the function formats this value and sends it over the serial connection.
    */
    void CTotalVoltage::_run()
    {
        if(!m_isActive) return;
        char buffer[one_byte];

        uint16_t pin_value = m_pin.read_u16();
        uint64_t l_rps = (pin_value*ref_A1_voltage_mV)/ref_A1_value_8925mV;

        int_globalsV_battery_totalVoltage = (uint16_t)l_rps;
        snprintf(buffer, sizeof(buffer), "@battery:%d;;\r\n", int_globalsV_battery_totalVoltage);
        m_serial.write(buffer,strlen(buffer));
    }

}; // namespace periodics
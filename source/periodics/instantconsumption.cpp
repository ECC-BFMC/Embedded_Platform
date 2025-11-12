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

#include <periodics/instantconsumption.hpp>

#define _24_chars 24
#define scaleFactor 1000
#define ADC_OFFSET_0A_SCALED_1000  6553500
#define adc_steps_per_1000mAmps_scaled_1000 524280

namespace periodics{
    /** \brief  Class constructor
     *
     *  It initializes the task and the state of the led. 
     *
     *  \param f_period       Toggling period of LED
     *  \param f_led          Digital output line to LED
     */
    CInstantConsumption::CInstantConsumption(
            std::chrono::milliseconds        f_period, 
            mbed::AnalogIn  f_pin, 
            UnbufferedSerial&      f_serial) 
        : utils::CTask(f_period)
        , m_pin(f_pin)
        , m_isActive(false)
        , m_serial(f_serial)
        , m_period(f_period.count())
    {
        
    }

    /** @brief  CInstantConsumption class destructor
     */
    CInstantConsumption::~CInstantConsumption()
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
    void CInstantConsumption::serialCallbackINSTANTcommand(char const * a, char * b) {
        uint8_t l_isActivate=0;
        uint8_t l_res = sscanf(a,"%hhu",&l_isActivate);

        if(1 == l_res){
            if(uint8_globalsV_value_of_kl == 15 || uint8_globalsV_value_of_kl == 30)
            {
                m_isActive=(l_isActivate>=1);
                bool_globalsV_instant_isActive = (l_isActivate>=1);
                sprintf(b,"1");
            }
            else{
                sprintf(b,"kl 15/30 is required!!");
            }
        }else{
            sprintf(b,"syntax error");
        }
    }

    /**
    * @brief Measures and filters the instantaneous current consumption, updating global tracking variables.
    * 
    * This function reads an analog input value representing current consumption, scales it to milliamps, and stores it in a circular buffer.
    * A moving average of the last `uint8_globalsV_windowSize` readings is computed to smooth out short-term fluctuations. Additionally, an Exponential Moving Average (EMA)
    * is updated to provide a weighted average of the current consumption over time. The total consumption and elapsed time are accumulated for further processing.
    *
    * The function is called periodically according to the task period (in milliseconds). It calculates both the raw instantaneous current and the smoothed
    * values (via moving average and EMA), which can be used for safety measures or consumption monitoring.
    * 
    * @param task_period The interval (in milliseconds) between each call to the function, used to accumulate total elapsed time.
    * 
    * Global Variables:
    * - `uint64_globalsV_instant_mAmpsH`: Stores the most recent instantaneous current value, scaled in milliamps.
    * - `uint32_globalsV_consumption_Total_mAmpsH`: Accumulates the total current consumption in milliamps over time.
    * - `uint32_globalsV_numberOfMiliseconds_Total`: Accumulates the total time passed (in milliseconds) for consumption calculations.
    * 
    * Steps:
    * 1. Read the current sensor (pin_value) and scale the result to milliamps.
    * 2. Store the current value in a circular buffer (size = `uint8_globalsV_windowSize`).
    * 3. Calculate the moving average (mean) over the last `uint8_globalsV_windowSize` readings.
    * 4. Update the Exponential Moving Average (EMA) to smooth fluctuations over time.
    * 5. Accumulate total current consumption and elapsed time for further analysis.
    */
    void CInstantConsumption::void_InstantSafetyMeasure(uint16_t task_period)
    {
        uint16_t pin_value = m_pin.read_u16();

        // char buffer[_24_chars];

        uint64_globalsV_instant_mAmpsH = pin_value * scaleFactor; // Scale for fix-point arithmetic

        if (uint64_globalsV_instant_mAmpsH <= ADC_OFFSET_0A_SCALED_1000) {
            uint64_globalsV_instant_mAmpsH = 0; // Ensure no negative values
        } else {
            uint64_globalsV_instant_mAmpsH -= ADC_OFFSET_0A_SCALED_1000;
        }

        if (uint64_globalsV_instant_mAmpsH <= 0) {
            uint64_globalsV_instant_mAmpsH = 0; // Ensure no negative values
        } else {
            uint64_globalsV_instant_mAmpsH *= scaleFactor; // Scale to mAmpsH

            uint64_globalsV_instant_mAmpsH /= adc_steps_per_1000mAmps_scaled_1000;
        }

        readings[uint8_globalsV_index % uint8_globalsV_windowSize] = (uint16_t)uint64_globalsV_instant_mAmpsH;
        uint8_globalsV_index++;

        int sum = 0;
        for (uint8_t i = 0; i < uint8_globalsV_windowSize; i++) {
            sum += readings[i];
        }
        
        int medianCurrent = sum / uint8_globalsV_windowSize;

        currentEMA = ((uint8_globalsV_alpha_scaled * medianCurrent) + ((scaleFactor - uint8_globalsV_alpha_scaled) * currentEMA))/scaleFactor;

        uint32_globalsV_consumption_Total_mAmpsH += uint64_globalsV_instant_mAmpsH;
        uint32_globalsV_numberOfMiliseconds_Total += task_period;
    }

    /**
    * @brief Periodically computes and sends the exponentially smoothed instantaneous consumption value over the serial connection.
    * 
    * When this function is active, it calculates the consumption value based on the Exponential Moving Average (EMA) of the instantaneous current readings.
    * The EMA is multiplied by the configured period (`m_period`) to scale the value to the desired time frame (milliseconds).
    * 
    * The result is then formatted and sent over a serial connection in a specific string format. The transmitted value represents the 
    * smoothed instantaneous consumption rate in milliamp-hours (mAh), converted from the EMA and task period.
    * 
    * Key Steps:
    * 1. Check if the component is active (`m_isActive`). If not, the function returns without performing any operations.
    * 2. Compute the scaled consumption value by multiplying the current Exponential Moving Average (`currentEMA`) by the period (`m_period`).
    * 3. Format the calculated value in milliamp-hours (mAh) as a string and send it over the serial connection.
    * 
    * @note This function is part of a periodic task and does not include any direct measurement or scaling from the A2 pin. It relies on the previously 
    *       updated `currentEMA` to smooth short-term fluctuations in consumption readings.
    * 
    * @param None
    * @return None
    */
    void CInstantConsumption::_run()
    {
        if(!m_isActive) return;

        char buffer[_24_chars];

        snprintf(buffer, sizeof(buffer), "@instant:%lu;;\r\n", currentEMA);
        m_serial.write(buffer,strlen(buffer));
    }

}; // namespace periodics
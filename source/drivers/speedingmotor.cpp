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

#include <drivers/speedingmotor.hpp>

namespace drivers{
    /**
     * @brief It initializes the pwm parameters and it sets the speed reference to zero position, and the limits of the car speed.
     * 
     * @param f_pwm_pin               pin connected to servo motor
     * @param f_inf_limit         inferior limit 
     * @param f_sup_limit         superior limit
     * 
     */
    CSpeedingMotor::CSpeedingMotor(
            PinName f_pwm_pin, 
            int f_inf_limit, 
            int f_sup_limit
        )
        : m_pwm_pin(f_pwm_pin)
        , m_inf_limit(f_inf_limit)
        , m_sup_limit(f_sup_limit)
    {
        // Set the ms_period on the pwm_pin
        m_pwm_pin.period_ms(ms_period); 
        // Set position to zero
        m_pwm_pin.pulsewidth_us(zero_default);
    };

    /** @brief  CSpeedingMotor class destructor
     */
    CSpeedingMotor::~CSpeedingMotor()
    {
    };

    /** @brief  It modifies the speed reference of the brushless motor, which controls the speed of the wheels. 
     *
     *  @param f_speed      speed in m/s, where the positive value means forward direction and negative value the backward direction. 
     */
    void CSpeedingMotor::setSpeed(int f_speed)
    {
        // char buffer[100];
        // snprintf(buffer, sizeof(buffer), "Initial value, Step Value: %d\r\n", step_value);
        // m_serial.write(buffer, strlen(buffer));

        step_value = interpolate(-f_speed, speedValuesP, speedValuesN, stepValues, 25);

        // snprintf(buffer, sizeof(buffer), "Speed requested: %d mm/s, Step Value: %d\n", f_speed, step_value);
        // m_serial.write(buffer, strlen(buffer));

        m_pwm_pin.pulsewidth_us(conversion(f_speed));
    };

    /** @brief  It puts the brushless motor into brake state, 
     */
    void CSpeedingMotor::setBrake()
    {
        m_pwm_pin.write(zero_default);
    };

    /**
    * @brief Interpolates values based on speed input.
    *
    * This function interpolates `stepValues` based on the provided `speed` input.
    * The interpolation is made using `steeringValueP` and `steeringValueN` as reference values.
    *
    * @param speed The input speed value for which the values need to be interpolated.
    * @param speedValuesP Positive reference values for speed.
    * @param speedValuesN Negative reference values for speed.
    * @param stepValues Step values corresponding to speedValueP and speedValueN which need to be interpolated.
    * @param size The size of the arrays.
    * @return The new value for the step value
    */
    // int16_t CSpeedingMotor::interpolate(int speed, const int speedValuesP[], const int speedValuesN[], const int stepValues[], int size)
    // {
    //     if(speed <= speedValuesP[0]){
    //         if (speed >= speedValuesN[0])
    //         {
    //             return stepValues[0];
    //         }
    //         else{
    //             for(uint8_t i=1; i<size; i++)
    //             {
    //                 if (speed >= speedValuesN[i])
    //                 {
    //                     int slope = (stepValues[i] - stepValues[i-1]) / (speedValuesN[i] - speedValuesN[i-1]);
    //                     return stepValues[i-1] + slope * (speed - speedValuesN[i-1]);
    //                 }
    //             }
    //         }
            
    //     } 
    //     if(speed >= speedValuesP[size-1]) return stepValues[size-1];
    //     if(speed <= speedValuesN[size-1]) return stepValues[size-1];

    //     for(uint8_t i=1; i<size; i++)
    //     {
    //         if (speed <= speedValuesP[i])
    //         {
    //             int slope = (stepValues[i] - stepValues[i-1]) / (speedValuesP[i] - speedValuesP[i-1]);
    //             return stepValues[i-1] + slope * (speed - speedValuesP[i-1]);
    //         }
    //     }

    //     return -1;
    // }
    int16_t CSpeedingMotor::interpolate(int speed, const int speedValuesP[], const int speedValuesN[], const int stepValues[], int size)
    {
        const int SCALE = 1000; // Factor de scalare pentru precizie
        // Pentru valorile negative
        if(speed <= speedValuesP[0]){
            if (speed >= speedValuesN[0])
            {
                return stepValues[0];
            }
            else {
                for(uint8_t i = 1; i < size; i++)
                {
                    if (speed >= speedValuesN[i])
                    {
                        int deltaStep = (stepValues[i] - stepValues[i-1]) * SCALE;
                        int deltaSpeed = speedValuesN[i] - speedValuesN[i-1];
                        int slope = deltaStep / deltaSpeed; // Calculăm panta în format fixed-point
                        int interpFixed = stepValues[i-1] * SCALE + slope * (speed - speedValuesN[i-1]);
                        return (int16_t)(interpFixed / SCALE);
                    }
                }
            }
        }
        if(speed >= speedValuesP[size-1]) return stepValues[size-1];
        if(speed <= speedValuesN[size-1]) return stepValues[size-1];

        // Pentru valorile pozitive
        for(uint8_t i = 1; i < size; i++)
        {
            if (speed <= speedValuesP[i])
            {
                int deltaStep = (stepValues[i] - stepValues[i-1]) * SCALE;
                int deltaSpeed = speedValuesP[i] - speedValuesP[i-1];
                int slope = deltaStep / deltaSpeed; // Panta în fixed-point
                int interpFixed = stepValues[i-1] * SCALE + slope * (speed - speedValuesP[i-1]);
                return (int16_t)(interpFixed / SCALE);
            }
        }
        return -1;
    }


    /** @brief  It converts speed reference to duty cycle for pwm signal. 
     * 
     *  @param f_speed    speed
     *  \return         pwm value
     */
    int CSpeedingMotor::conversion(int f_speed)
    {   
        return ((step_value * f_speed)/100 + zero_default);
    };

    /**
     * @brief It verifies whether a number is in the given treshold
     * 
     * @param f_speed value 
     * @return true means, that the value is in the range
     * @return false means, that the value isn't in the range
     */
    bool CSpeedingMotor::inRange(int f_speed){
        return (m_inf_limit<=f_speed) && (f_speed<=m_sup_limit);
    };
}; // namespace hardware::drivers
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


#include <hardware/encoders/quadratureencoder.hpp>


namespace hardware::encoders{

    /**
     * @brief Construct a new CQuadratureEncoderTask::CQuadratureEncoderTask object
     * 
     * @param f_period_sec          Period of the task
     * @param f_quadraturecounter   The counter object
     * @param f_resolution          The resolution of the rotation encoder. (Cpr count per revolution)
     */
    CQuadratureEncoder::CQuadratureEncoder(   float                           f_period_sec
                                                    ,hardware::encoders::IQuadratureCounter_TIMX*        f_quadraturecounter
                                                    ,uint16_t                        f_resolution)
                                                    :m_quadraturecounter(f_quadraturecounter)
                                                    ,m_encoderCnt()
                                                    ,m_encoderStps(0)
                                                    ,m_encoderStpsEnable(false)
                                                    ,m_taskperiod_s(f_period_sec)
                                                    ,m_resolution(f_resolution)
                                                    ,m_timer()
    {
    }


    /**
     * @brief Start periodically the '_run' function. 
     * 
     */
    void CQuadratureEncoder::startTimer(){
        m_timer.attach(mbed::callback(this,&CQuadratureEncoder::_run), m_taskperiod_s);
    }


    /**
     * @brief The run function will be applied periodically. 
     * 
     */
    void CQuadratureEncoder::_run(){
        m_encoderCnt = m_quadraturecounter->getCount();
        if (m_encoderStpsEnable)
        {
            m_encoderStps += m_encoderCnt;
        }
        m_quadraturecounter->reset();
    }

    /**
     * @brief Getter function for counted impluses in the last period.
     * 
     * @return Counted impulses
     */
    int16_t CQuadratureEncoder::getCount(){
        return m_encoderCnt;
    }

    /**
     * @brief Getter function for the rotation speed (rotation per second)
     * 
     * @return Rotation speed (rotation per second)
     */

    float CQuadratureEncoder::getSpeedRps(){
        return static_cast<float>(m_encoderCnt)/ m_resolution / m_taskperiod_s;
    }

    /**
     * @brief Getter function for the traveled distance. 
     * 
     * @return traveled distance
     */
    float CQuadratureEncoder::getTraveledDistance(){
        return static_cast<double>(m_encoderStps);
    }

    /**
     * @brief Getter function starts the recording for the traveled distance. 
     * 
     * @return 
     */
    void CQuadratureEncoder::startDistMeasure(){
        m_encoderStps = 0;
        m_encoderStpsEnable = true;
    }

    /**
     * @brief Getter function for the traveled distance. 
     * 
     * @return 
     */
    void CQuadratureEncoder::stopDistMeasure(){
        m_encoderStps = 0;
        m_encoderStpsEnable = false;
    }

    /**
     * @brief Construct a new CQuadratureEncoderWithFilterTask::CQuadratureEncoderWithFilterTask object
     * 
     * @param f_period_sec Period of the task
     * @param f_quadraturecounter The counter object
     * @param f_resolution The resolution of the rotation encoder. (Cpr count per revolution)
     * @param f_filter The reference to the filter. 
     */
    CQuadratureEncoderWithFilter::CQuadratureEncoderWithFilter(   float                           f_period_sec
                                                                        ,hardware::encoders::IQuadratureCounter_TIMX*        f_quadraturecounter
                                                                        ,uint16_t                       f_resolution
                                                                        ,signal::filter::IFilter<float>&       f_filter)
                                                                        :CQuadratureEncoder(f_period_sec,f_quadraturecounter,f_resolution)
                                                                        ,m_filter(f_filter)
                                                                        {
    }

    /**
     * @brief  The '_run' method aims for getting the value from the counter and reseting it.  In the last step, it filters the measured values. 
     * This method is applied automatically and periodically by the rtos timer, if it was started by the method 'startTimer'.
     * 
     */
    void CQuadratureEncoderWithFilter::_run(){
        CQuadratureEncoder::_run();
        float temp = m_encoderCnt;
        m_encoderCntFiltered = static_cast<int16_t>(m_filter(temp));
    }

    /**
     * @brief Getter function for the last filtered value.
     * 
     * @return Filtered counted impulses 
     */
    int16_t CQuadratureEncoderWithFilter::getCount(){
        return m_encoderCntFiltered;
    }

    /**
     * @brief Getter function for the last filtered rotation speed (rotation per second). 
     * 
     * @return Filtered rotation speed in rps 
     */
    float CQuadratureEncoderWithFilter::getSpeedRps(){
        return static_cast<double>(m_encoderCntFiltered)/m_resolution / m_taskperiod_s;

    }

    /**
     * @brief Getter function for the last non-filtered value. 
     *
     * @return Non-filtered counted impulses  
     */
    int16_t CQuadratureEncoderWithFilter::getNonFilteredCount(){
        return m_encoderCntFiltered;
    }

    /**
     * @brief Getter function for the last non-filtered rotation speed. 
     *
     * @return Non-filtered rotation speed in rps  
     */
    float CQuadratureEncoderWithFilter::getNonFilteredSpeedRps(){
        return static_cast<double>(m_encoderCntFiltered)/m_resolution / m_taskperiod_s;

    }
    
}; // namespace hardware::encoders 
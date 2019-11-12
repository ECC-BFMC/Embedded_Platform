/**
Copyright 2019 Bosch Engineering Center Cluj and BFMC organizers 

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

 * @file quadratureencodertask.cpp
 * @author RBRO/PJ-IU
 * @brief 
 * @version 0.1
 * @date 2018-10-23
 * 
 * @copyright Copyright (c) 2018
 * 
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
                                                ,hardware::drivers::IQuadratureCounter_TIMX*        f_quadraturecounter
                                                ,uint16_t                        f_resolution)
                                                :m_quadraturecounter(f_quadraturecounter)
                                                ,m_taskperiod_s(f_period_sec)
                                                ,m_resolution(f_resolution)
                                                ,m_timer(mbed::callback(this,&CQuadratureEncoder::_run))
{
}


/**
 * @brief Start the RosTimer to periodically apply the '_run' function. 
 * 
 */
void CQuadratureEncoder::startTimer(){
    m_timer.start(static_cast<int>(m_taskperiod_s*1000));
}


/**
 * @brief The run function will be applied periodically. 
 * 
 */
void CQuadratureEncoder::_run(){
    m_encoderCnt = m_quadraturecounter->getCount();
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
 * @brief Construct a new CQuadratureEncoderWithFilterTask::CQuadratureEncoderWithFilterTask object
 * 
 * @param f_period_sec Period of the task
 * @param f_quadraturecounter The counter object
 * @param f_resolution The resolution of the rotation encoder. (Cpr count per revolution)
 * @param f_filter The reference to the filter. 
 */
CQuadratureEncoderWithFilter::CQuadratureEncoderWithFilter(   float                           f_period_sec
                                                                    ,hardware::drivers::IQuadratureCounter_TIMX*        f_quadraturecounter
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
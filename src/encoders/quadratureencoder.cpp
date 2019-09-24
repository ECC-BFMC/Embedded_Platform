/**
 * @file quadratureencodertask.cpp
 * @author RBRO/PJ-IU
 * @brief 
 * @version 0.1
 * @date 2018-10-23
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#include <encoders/quadratureencoder.hpp>


namespace encoders{



/**
 * @brief Construct a new CQuadratureEncoderTask::CQuadratureEncoderTask object
 * 
 * @param f_period_sec Period of the task
 * @param f_Quadratureencoder The counter object
 * @param f_resolution The resolution of the rotation encoder. (Cpr count per revolution)
 */
CQuadratureEncoder::CQuadratureEncoder(   float                           f_period_sec
                                                ,drivers::IQuadratureCounter_TIMX*        f_quadraturecounter
                                                ,uint16_t                        f_resolution)
                                                :m_quadraturecounter(f_quadraturecounter)
                                                ,m_taskperiod_s(f_period_sec)
                                                ,m_resolution(f_resolution)
                                                ,m_timer(mbed::callback(this,&CQuadratureEncoder::_run))
{
}


/**
 * @brief Start the RosTimer to periodically apply the _run function. 
 * 
 */
void CQuadratureEncoder::startTimer(){
    m_timer.start(static_cast<int>(m_taskperiod_s*1000));
}


/**
 * @brief The private run function, which will be applied periodically. 
 * 
 */
void CQuadratureEncoder::_run(){
    m_encoderCnt = m_quadraturecounter->getCount();
    m_quadraturecounter->reset();
}

/**
 * @brief Getter function for counted impluses in the last period.
 * 
 * @return int16_t - counted impulses
 */
int16_t CQuadratureEncoder::getCount(){
    return m_encoderCnt;
}
/**
 * @brief Getter function for the rotation speed (rotation per second)
 * 
 * @return float - rotation speed (rotation per second)
 */

float CQuadratureEncoder::getSpeedRps(){
    return static_cast<float>(m_encoderCnt)/ m_resolution / m_taskperiod_s;
}

/**
 * @brief Construct a new CQuadratureEncoderWithFilterTask::CQuadratureEncoderWithFilterTask object
 * 
 * @param f_period_sec Period of the task
 * @param f_Quadratureencoder The counter object
 * @param f_resolution The resolution of the rotation encoder. (Cpr count per revolution)
 * @param f_filter The reference to the filter. 
 */
CQuadratureEncoderWithFilter::CQuadratureEncoderWithFilter(   float                           f_period_sec
                                                                    ,drivers::IQuadratureCounter_TIMX*        f_quadraturecounter
                                                                    ,uint16_t                       f_resolution
                                                                    ,filter::CFilterFunction<float>&       f_filter)
                                                                    :CQuadratureEncoder(f_period_sec,f_quadraturecounter,f_resolution)
                                                                    ,m_filter(f_filter)
                                                                    {
}

/**
 * @brief Private run method for getting the value from the counter, reseting it.  In the last step, it filters the measured values. 
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
 * @return int16_t - filtered counted impulses 
 */
int16_t CQuadratureEncoderWithFilter::getCount(){
    return m_encoderCntFiltered;
}

/**
 * @brief Getter function for the last filtered rotation speed (rotation per second). 
 * 
 * @return float - filtered rotation speed in rps 
 */
float CQuadratureEncoderWithFilter::getSpeedRps(){
    return static_cast<double>(m_encoderCntFiltered)/m_resolution / m_taskperiod_s;

}

/**
 * @brief Getter function for the last non-filtered value. 
 *
 * @return int16_t - non-filtered counted impulses  
 */
int16_t CQuadratureEncoderWithFilter::getNonFilteredCount(){
    return m_encoderCntFiltered;
}

/**
 * @brief Getter function for the last non-filtered rotation speed. 
 *
 * @return float - non-filtered rotation speed in rps  
 */
float CQuadratureEncoderWithFilter::getNonFilteredSpeedRps(){
    return static_cast<double>(m_encoderCntFiltered)/m_resolution / m_taskperiod_s;

}


}; // namespace encoders 
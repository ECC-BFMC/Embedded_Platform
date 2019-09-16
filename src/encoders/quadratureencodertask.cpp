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
#include <Encoders/Quadratureencodertask.hpp>


namespace encoders{



/**
 * @brief Construct a new CQuadratureEncoderTask::CQuadratureEncoderTask object
 * 
 * @param f_period_sec Period of the task
 * @param f_Quadratureencoder The counter object
 * @param f_resolution The resolution of the rotation encoder. (Cpr count per revolution)
 */
CQuadratureEncoderTask::CQuadratureEncoderTask(   float                           f_period_sec
                                                ,encoders::CQuadratureEncoder_TIMX*        f_Quadratureencoder
                                                ,uint16_t                        f_resolution)
                                                :m_Quadratureencoder(f_Quadratureencoder)
                                                ,m_taskperiod_s(f_period_sec)
                                                ,m_resolution(f_resolution)
                                                ,m_timer(mbed::callback(CQuadratureEncoderTask::static_callback,this))
{
}


/**
 * @brief Start the RosTimer to periodically apply the _run function. 
 * 
 */
void CQuadratureEncoderTask::startTimer(){
    m_timer.start(static_cast<int>(m_taskperiod_s*1000));
}


/**
 * @brief The private run function, which will be applied periodically. 
 * 
 */
void CQuadratureEncoderTask::_run(){
    m_encoderCnt = m_Quadratureencoder->getCount();
    m_Quadratureencoder->reset();
}
/**
 * @brief Static callback function for applying the run function. 
 * 
 * @param obj CQuadratureEncoderTask object
 */
void CQuadratureEncoderTask::static_callback(void* obj){
    CQuadratureEncoderTask* self = static_cast<CQuadratureEncoderTask*>(obj);
    self->_run();
}

/**
 * @brief Getter function for counted value.
 * 
 * @return int16_t counted impulse
 */
int16_t CQuadratureEncoderTask::getCount(){
    return m_encoderCnt;
}
/**
 * @brief Getter function for the rotation per second
 * 
 * @return rotation speed (rotation per second)
 */

float CQuadratureEncoderTask::getSpeedRps(){
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
CQuadratureEncoderWithFilterTask::CQuadratureEncoderWithFilterTask(   float                           f_period_sec
                                                                    ,encoders::CQuadratureEncoder_TIMX*        f_Quadratureencoder
                                                                    ,uint16_t                       f_resolution
                                                                    ,filter::CFilterFunction<float>&       f_filter)
                                                                    :CQuadratureEncoderTask(f_period_sec,f_Quadratureencoder,f_resolution)
                                                                    ,m_filter(f_filter)
                                                                    {
}

/**
 * @brief Private run method for getting the value from the counter, reseting it.  In the last step it filters the readed values. 
 * 
 */
void CQuadratureEncoderWithFilterTask::_run(){
    m_encoderCnt = m_Quadratureencoder->getCount();
    m_Quadratureencoder->reset();
    float temp = m_encoderCnt;
    m_encoderCntFiltered = static_cast<int16_t>(m_filter(temp));

}

/**
 * @brief Getter function for the last filtered value. 
 * 
 */
int16_t CQuadratureEncoderWithFilterTask::getCount(){
    return m_encoderCntFiltered;
}

/**
 * @brief Getter function for the last filtered rotation speed. 
 * 
 */
float CQuadratureEncoderWithFilterTask::getSpeedRps(){
    return static_cast<double>(m_encoderCntFiltered)/m_resolution / m_taskperiod_s;

}

/**
 * @brief Getter function for the last non-filtered value. 
 * 
 */
int16_t CQuadratureEncoderWithFilterTask::getNonFilteredCount(){
    return m_encoderCntFiltered;
}

/**
 * @brief Getter function for the last non-filtered rotation speed. 
 * 
 */
float CQuadratureEncoderWithFilterTask::getNonFilteredSpeedRps(){
    return static_cast<double>(m_encoderCntFiltered)/m_resolution / m_taskperiod_s;

}


}; // namespace encoders 
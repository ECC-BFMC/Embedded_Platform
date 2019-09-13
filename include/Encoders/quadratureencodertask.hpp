/**
 * @file quadratureencodertask.hpp
 * @author  RBRO/PJ-IU
 * @brief 
 * @version 0.1
 * @date 2018-10-23
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#ifndef QUADRATURE_ENCODER_TASK_HPP
#define QUADRATURE_ENCODER_TASK_HPP

#include <Encoders/encoderinterface.hpp>
#include <Encoders/QuadratureEncoder.hpp>
#include <Filter/Filter.hpp>

#include <rtos.h>

namespace encoders{

/**
 * @brief Rotary encoder with filter interface class
 * 
 */
class CQuadratureEncoderNonFilteredGetterInterface{
    public:
      virtual int16_t  getNonFilteredCount()=0;
      virtual float getNonFilteredSpeedRps() = 0;
};

/**
 * @brief It implements a periodic task, which get the value from the counter and reset it to zero.
 * 
 */
class CQuadratureEncoderTask:public IEncoderGetter{
    public:
      CQuadratureEncoderTask(float,encoders::CQuadratureEncoder_TIMX*,uint16_t);
      static void static_callback(void* obj);

      void startTimer();
  protected:
    virtual void _run();
    virtual int16_t getCount();
    virtual float getSpeedRps();
    virtual bool isAbs(){return false;}
  protected:
      encoders::CQuadratureEncoder_TIMX *m_Quadratureencoder;
      int16_t           m_encoderCnt;
      const float       m_taskperiod_s;
      const uint16_t    m_resolution;
      RtosTimer         m_timer;
};

/**
 * @brief It implements the same functionality than CQuadratureEncoderTask class, but in additional it can filter the values. 
 * 
 */
class CQuadratureEncoderWithFilterTask: public CQuadratureEncoderTask, public CQuadratureEncoderNonFilteredGetterInterface{
    public:
      CQuadratureEncoderWithFilterTask(float,encoders::CQuadratureEncoder_TIMX *, uint16_t,filter::CFilterFunction<float>&);
      virtual void _run();
      virtual int16_t getCount();
      virtual float getSpeedRps();
      virtual int16_t  getNonFilteredCount();
      virtual float getNonFilteredSpeedRps();
    protected:
      double m_encoderCntFiltered;
      filter::CFilterFunction<float>& m_filter;

};

}; // namespace encoders

#endif
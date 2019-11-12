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
 
 * @file quadratureencodertask.hpp
 * @author  RBRO/PJ-IU
 * @brief 
 * @version 0.1
 * @date 2018-10-23
 * 
 */
#ifndef QUADRATURE_ENCODER_TASK_HPP
#define QUADRATURE_ENCODER_TASK_HPP

#include <hardware/encoders/encoderinterfaces.hpp>
#include <hardware/encoders/quadraturecounter.hpp>
#include <signal/filter/filter.hpp>

#include <rtos.h>

namespace hardware::encoders{

/**
 * @brief It implements a periodic task, which get the value from the counter and reset it to zero.
 * 
 */
class CQuadratureEncoder:public IEncoderGetter{
  public:
      CQuadratureEncoder(float,hardware::drivers::IQuadratureCounter_TIMX*,uint16_t);
      void startTimer();
    virtual void _run();
    virtual int16_t getCount();
    virtual float getSpeedRps();
    virtual bool isAbs(){return false;}
  protected:
      /** @brief Counter interface */
      ::hardware::drivers::IQuadratureCounter_TIMX *m_quadraturecounter;
      /** @brief Last counted value */
      int16_t           m_encoderCnt;
      /** @brief Sampling period */
      const float       m_taskperiod_s;
      /** @brief Resolution of encoder */
      const uint16_t    m_resolution;
      /** @brief Rtos Timer for periodically applying */
      RtosTimer m_timer;
};

/**
 * @brief It implements the same functionality than CQuadratureEncoderTask class, but in additional it can filter the values. 
 * 
 */
class CQuadratureEncoderWithFilter: public CQuadratureEncoder, public IEncoderNonFilteredGetter{
    public:
      CQuadratureEncoderWithFilter(float,hardware::drivers::IQuadratureCounter_TIMX *, uint16_t,signal::filter::IFilter<float>&);
      
      virtual int16_t getCount();
      virtual float getSpeedRps();
      virtual int16_t  getNonFilteredCount();
      virtual float getNonFilteredSpeedRps();
    protected:
      virtual void _run();
      /** @brief Last filtered counted value */
      double m_encoderCntFiltered;
      /** @brief Filter interface */
      signal::filter::IFilter<float>& m_filter;

};

}; // namespace hardware::encoders

#endif
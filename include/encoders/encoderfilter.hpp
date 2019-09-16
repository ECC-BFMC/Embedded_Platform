/**
  ******************************************************************************
  * @file    EncoderFilter.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class declaration for the encoder filter
  *          functionality.
  ******************************************************************************
 */

/* Include guard */
#ifndef ENCODER_FILTER_HPP
#define ENCODER_FILTER_HPP
#include <Encoders/encoder.hpp>
#include <Filter/filter.hpp>

//!  CEncoderFilter class. 
/*!
 * It inherits class CEncoder. 
 * It is used for obtaining filtered encoder output.
 */
class CEncoderFilter:public CEncoder
{
    /* Filter object */
    template <class T>
    using CFilterFunction = filter::CFilterFunction<T>;
    public:
        /* Constructor */
        CEncoderFilter  (uint32_t                       f_period
                        ,float                          f_period_sec
                        ,uint32_t                       f_cpr
                        ,PinName                        f_pinName
                        ,CFilterFunction<float>&        f_filter);
        
        virtual float getSpeedRps();
        virtual int16_t  getCount() = 0;
    protected:
        /* Run method */
        virtual void _run();
        
    private:
        /* Filter member */
        CFilterFunction<float>&    m_filter;
        /* Value of filtered rotations per second */
        float                      m_filteredRps;
};

#endif
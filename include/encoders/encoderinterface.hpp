/**
 * @file encoderinterface.hpp
 * @author RBRO/PJ-IU
 * @brief 
 * @version 0.1
 * @date 2018-10-24
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#ifndef ENDOCER_INTERFACE_HPP
#define ENDOCER_INTERFACE_HPP

#include <stdint.h>
namespace encoders{
    /**
     * @brief Rotary encoder interface class
     *
     */
    class IEncoderGetter{
        public:
        /**
         * @brief Get the Count value
         * 
         * @return int16_t 
         */
        virtual int16_t  getCount() = 0;
        /**
         * @brief Get the Speed Rps 
         * 
         * @return float 
         */
        virtual float getSpeedRps() = 0;
        /**
         * @brief Get the encoder capability. If it's true, than the encoder can give the oriantation, else it returns the absolute value of the rotation without the oriantation. 
         * 
         * @return bool 
         */
        virtual bool isAbs() = 0;
    };
}; // namepsace encoders;

#endif
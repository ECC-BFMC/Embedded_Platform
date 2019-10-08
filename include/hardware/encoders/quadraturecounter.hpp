/**
 * @file quadratureencoder.hpp
 * @author RBRO/PJ-IU
 * @brief 
 * @version 0.1
 * @date 2018-10-23
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#ifndef QUADRATURE_COUNTER__HPP
#define QUADRATURE_COUNTER__HPP

#include <mbed.h>

namespace hardware::drivers{
  /**
   * @brief  An interface to access the main functionality of timer for decoding a quadrature encoder. It's used to get and to reset the measured position of the encoder.
   * 
   */
  class IQuadratureCounter_TIMX{
      public:
        virtual int16_t getCount() = 0;
        virtual void reset() = 0;
  }; // class IQuadratureCounter_TIMX

  /**
   * @brief A driver for quadrature encoder based on timer TIM4. 
   * 
   * It's a singleton class for receiving and decoding the Quadrature signal. 
   * It returns the position of the encoder via method 'getCount' and sets the position to zero by method 'reset'. 
   */
  class CQuadratureCounter_TIM4:public IQuadratureCounter_TIMX{
      
      /**
       * @brief It uses to destroy the singleton object.
       */
      class CQuadratureCounter_TIM4_Destroyer{
      public:
          CQuadratureCounter_TIM4_Destroyer(){}
          ~CQuadratureCounter_TIM4_Destroyer();
          void SetSingleton(CQuadratureCounter_TIM4* s);
      private:
          CQuadratureCounter_TIM4* m_singleton;
      }; // class CQuadratureCounter_TIM4_Destroyer

    public:
      static CQuadratureCounter_TIM4 *Instance();
      int16_t getCount();
      void reset();
      virtual ~CQuadratureCounter_TIM4() {}
    protected:
      CQuadratureCounter_TIM4() {}
    
    private:
      void initialize();
      static CQuadratureCounter_TIM4* m_instance;
      static CQuadratureCounter_TIM4_Destroyer m_destroyer;
  }; //class CQuadratureCounter_TIM4

};// namespace hardware::drivers


#endif
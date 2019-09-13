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

#ifndef Quadrature_ENCODER_HPP
#define Quadrature_ENCODER_HPP

#include <mbed.h>

namespace encoders{
/**
 * @brief  It an interface for accessing the encoder position. It can be used to get and to reset the position of the encoder.
 * 
 */
class CQuadratureEncoder_TIMX{
    public:
      virtual int16_t getCount() = 0;
      virtual void reset() = 0;
};

/**
 * @brief It's a singleton class for receiving and decoding the Quadrature signal by using the timer TIM4. It can get the direction of the rotation.
 * 
 */
class CQuadratureEncoder_TIM4:public CQuadratureEncoder_TIMX{
    /**
     * @brief It uses to destroy the singleton class.
     * 
     */
    class CQuadratureEncoder_TIM4_Destroyer{
    public:
        CQuadratureEncoder_TIM4_Destroyer(){}
        ~CQuadratureEncoder_TIM4_Destroyer();
        void SetSingleton(CQuadratureEncoder_TIM4* s);
    private:
         CQuadratureEncoder_TIM4* m_singleton;
    };

  public:
    static CQuadratureEncoder_TIM4 *Instance();
    int16_t getCount();
    void reset();
    virtual ~CQuadratureEncoder_TIM4() {}
  protected:
    CQuadratureEncoder_TIM4() {}
   
  private:
    void initialize();
    static CQuadratureEncoder_TIM4* m_instance;
    static CQuadratureEncoder_TIM4_Destroyer m_destroyer;
};

};// namepsace drivers


#endif
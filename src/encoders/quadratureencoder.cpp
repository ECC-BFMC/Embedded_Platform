/**
 * @file quadratureencoder.cpp
 * @author RBRO/PJ-IU
 * @brief 
 * @version 0.1
 * @date 2018-10-23
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#include <Encoders/Quadratureencoder.hpp>


namespace encoders{



CQuadratureEncoder_TIM4 *CQuadratureEncoder_TIM4::m_instance = 0;
CQuadratureEncoder_TIM4::CQuadratureEncoder_TIM4_Destroyer CQuadratureEncoder_TIM4::m_destroyer;


/**
 * @brief Setter function.
 * 
 * @param s singleton object address
 */
void CQuadratureEncoder_TIM4::CQuadratureEncoder_TIM4_Destroyer::SetSingleton(CQuadratureEncoder_TIM4* s){
    m_singleton = s;
}  

/**
 * @brief Destroy the cQuadratureencoder tim4::cQuadratureencoder tim4 destroyer::cQuadratureencoder tim4 destroyer object
 * 
 */
CQuadratureEncoder_TIM4::CQuadratureEncoder_TIM4_Destroyer::~CQuadratureEncoder_TIM4_Destroyer(){
    delete m_singleton;
}


/**
 * @brief Constructor function. It verifies the existance of the singleton object and creates it.
 * 
 * @return The address of the singleton object
 */
CQuadratureEncoder_TIM4* CQuadratureEncoder_TIM4::Instance(){
    if(!CQuadratureEncoder_TIM4::m_instance){
        CQuadratureEncoder_TIM4::m_instance = new CQuadratureEncoder_TIM4;
        m_instance->initialize();
        CQuadratureEncoder_TIM4::m_destroyer.SetSingleton(m_instance);
    }
    return CQuadratureEncoder_TIM4::m_instance;
}


/**
 * @brief Initialize the parameter of the object.
 * 
 */
void CQuadratureEncoder_TIM4::initialize(){
    //PB6 PB7 aka D10 MORPHO_PB7
    // Enable clock for GPIOA
   RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

   //stm32f4xx.h
   GPIOB->MODER |= GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1;         //PB6 & PB7 as Alternate Function   /*!< GPIO port mode register,               Address offset: 0x00      */
   GPIOB->OTYPER |= GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7;              //PB6 & PB7 as Inputs               /*!< GPIO port output type register,        Address offset: 0x04      */
   GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7; //Low speed                         /*!< GPIO port output speed register,       Address offset: 0x08      */
   GPIOB->PUPDR |= GPIO_PUPDR_PUPDR6_1 | GPIO_PUPDR_PUPDR7_1;         //Pull Down                         /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
   GPIOB->AFR[0] |= 0x22000000;                                       //AF02 for PB6 & PB7                /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
   GPIOB->AFR[1] |= 0x00000000;                                       //nibbles here refer to gpio8..15   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */

   // configure TIM4 as Encoder input
   // Enable clock for TIM4
   // __TIM4_CLK_ENABLE();
   RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

   TIM4->CR1 = 0x0001;                // CEN(Counter ENable)='1'     < TIM control register 1
   TIM4->SMCR = TIM_ENCODERMODE_TI12; //               < TIM slave mode control register
   //TIM_ENCODERMODE_TI1 input 1 edges trigger count
   //TIM_ENCODERMODE_TI2 input 2 edges trigger count
   //TIM_ENCODERMODE_TI12 all edges trigger count
   TIM4->CCMR1 = 0xF1F1; // CC1S='01' CC2S='01'         < TIM capture/compare mode register 1
   //0xF nibble sets up filter
   TIM4->CCMR2 = 0x0000;                       //                             < TIM capture/compare mode register 2
   TIM4->CCER = TIM_CCER_CC1E | TIM_CCER_CC2E; //     < TIM capture/compare enable register
   TIM4->PSC = 0x0000;                         // Prescaler = (0+1)           < TIM prescaler
   TIM4->ARR = 0xffff;                         // reload at 0xfffffff         < TIM auto-reload register

   TIM4->CNT = 0x0000; //reset the counter before we use it
}
/**
 * @brief Get the position of encoder. 
 * 
 */
int16_t CQuadratureEncoder_TIM4::getCount(){
    if(m_instance){
        return TIM4->CNT;
    }
    return 0xffff;
}
/**
 * @brief Reset the value of the counter to zero value.
 */
void CQuadratureEncoder_TIM4::reset(){
    if(m_instance){
        TIM4->CNT = 0;
    }
}


}; // namespace drivers
/**
  ******************************************************************************
  * @file    Ekf_am.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class implementation for the Ackermann model example
  *          functionality.
  ******************************************************************************
 */

#include <Examples/Filter/ekf_am.hpp>

/** \brief  Constructor for the CEKF_AM class
 *
 *  Constructor method
 *
 *  \param[in] f_ackermannModel       reference to Ackermann model object    
 *  \param[in] f_jacobianMatrixCalc   Ackermann model Jacobian matrix
 *  \param[in] f_Q                    reference to transition type
 *  \param[in] f_R                    reference to observation noise type
 */
examples::filter::CEKF_AM::CEKF_AM(CAckermannModel&              f_ackermannModel
                                  ,CJMAckermannModel             f_jacobianMatrixCalc
                                  ,const CJMTransitionType&      f_Q
                                  ,const CObservationNoiseType&  f_R)
    :CEKF<double,2,10,5>(f_ackermannModel,f_jacobianMatrixCalc,f_Q,f_R)
{
    
}
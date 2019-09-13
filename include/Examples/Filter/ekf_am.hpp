/**
  ******************************************************************************
  * @file    Ekf_am.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class declaration for the Ackermann model example
  *          functionality.
  ******************************************************************************
 */

/* Include guard */
#ifndef EKF_AM_HPP
#define EKF_AM_HPP

#include <Filter/filter.hpp>
#include <Examples/SystemModels/ackermannmodel.hpp>

namespace examples
{
    namespace filter
    {
        /* Extended Kalman Filter */
        template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
        using CEKF  =   ::filter::ltv::mimo::CEKF<T,NA,NB,NC>;
        //!  CEKF_AM class. 
        /*!
        * It inherits class CEKF. 
        * Extended Kalman Filter aplied on the Ackermann Model.
        */
        class CEKF_AM:public CEKF<double,2,10,5>
        {
            private:
                /* Ackermann model*/
                using CAckermannModel = ::examples::systemmodels::ackermannmodel::CAckermannModel;
                /* Ackermann model Jacobian Matrix */
                using CJMAckermannModel = ::examples::systemmodels::ackermannmodel::CJMAckermannModel;
            public:
                /* Constructor */
                CEKF_AM(CAckermannModel&              f_ackermannModel
                        ,CJMAckermannModel             f_jacobianMatrixCalc
                        ,const CJMTransitionType&      f_Q
                        ,const CObservationNoiseType&  f_R);
        };
    };
};

#endif
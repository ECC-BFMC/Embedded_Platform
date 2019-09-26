/**
 * @file kalmanfilters.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-09-26
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef KALMAN_FILTERS_HPP
#define KALMAN_FILTERS_HPP

#include <linalg/linalg.h>
#include <systemmodels/systemmodels.hpp>

namespace filter{
    namespace lti{
        namespace mimo{
            
            /**
             * @brief General implementation of Kalman filter for a linear time invariant systems with or without control signals. 
             * 
             * @tparam T    The type of the input and output signal (floalt,double)
             * @tparam NA   Number of state values
             * @tparam NB   Number of control values
             * @tparam NC   Number of observation values
             */
            template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
            class CKalmanFilter
            {
            public:
                // Definning the types.
                using CStateType                    = linalg::CVector<T, NA>;  // State column matrix 
                using CControlType                  = linalg::CVector<T, NB>;  // Control column matrix 
                using CObservationType              = linalg::CVector<T, NC>;  // Observation column matrix 
                
                using CStateTransModelType          = linalg::CMatrix<T, NA, NA>; // State transition type 
                using CControInputModelType         = linalg::CMatrix<T, NA, NC>; // Control-input model, applied to control vecter
                using CObservationModelType         = linalg::CMatrix<T,NC,NA>;   // Observation model type

                using CProcessCovarianceType        = linalg::CMatrix<T, NA, NA>;
                using CObservationCovarianceType    = linalg::CMatrix<T, NC, NC>;
                
                using CKalmanGainType               = linalg::CMatrix<T, NA, NC>;

                /* Constructor */
                CKalmanFilter(
                    const CStateTransModelType&             f_stateTransitionModel,
                    const CControInputModelType&            f_controlInput,
                    const CObservationModelType&            f_observationModel,
                    const CProcessCovarianceType&           f_covarianceProcessNoise,
                    const CObservationCovarianceType&       f_observationNoiseCovariance);

                /* Get state method */
                const CStateType& state() const
                {
                    return m_state;
                }
                /* Get state method */
                CStateType& state()
                {
                    return m_state;
                }
                /* Operator */
                CObservationType operator()(const CControlType& f_control,const CObservationType& f_observation);
                /* Operator */
                CObservationType operator()(const CObservationType& f_observation);
                /* Predict */
                void predict();
                /* Predict */
                void predict(const CControlType& f_input);
                /* Update */
                CObservationType update(const CObservationType& f_observation);

                private:
                CKalmanFilter() {}

                CStateTransModelType            m_stateTransitionModel;         // state transition model
                CControInputModelType           m_controlInputModel;            // control-input model
                CObservationModelType           m_observationModel;             // observation model

                CStateType                      m_state;                        // priori state
                
                CProcessCovarianceType          m_processCovariance;            // priori covariance estimate           
                CKalmanGainType                 m_kalmanGain;                   // optimal kalman gain    
                
                CProcessCovarianceType          m_covarianceProcessNoise;       // covariance of process noise          
                CObservationCovarianceType      m_observationNoiseCovariance;   // covariance of observation noise                     
            }; // Class CKalmanFilter
        }; // mimo
    }; // namespace lti

    namespace nlti{
        namespace mimo{
            /**
             * @brief The state transition and observation matrices defined by Jacobians
             * 
             * @tparam T    variable type
             * @tparam NA   number of control values
             * @tparam NB   number of state values
             * @tparam NC   number of observation values
             */
            template <class T,uint32_t NA, uint32_t NB,uint32_t NC>
            class IJacobianMatrices{
                public:
                    using CStatesType           =   linalg::CMatrix<T,NB,1>;
                    using CControlType            =   linalg::CMatrix<T,NA,1>;
                    // using CObservationType           =   linalg::CMatrix<T,NC,1>;
                    using CJMTransitionType     =   linalg::CMatrix<T,NB,NB>;
                    using CJMObservationType    =   linalg::CMatrix<T,NC,NB>;

                    //The method calculates and returns  the states transition matrix 
                    virtual CJMTransitionType getJMTransition(const CStatesType&     f_states
                                                             ,const CControlType&      f_inputs)=0;
                    // The method calculates and returns  the states observation matrix
                    virtual CJMObservationType getJMObservation(const CStatesType&      f_states
                                                               ,const CControlType&       f_input)=0;
                private:
            }; // class IJacobianMatrices
            
            /**
             * @brief Extended Kalman Filter
             * 
             * @tparam T    variable type
             * @tparam NA   number of control values
             * @tparam NB   number of state values
             * @tparam NC   number of observation values
             */
            template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
            class CEKF
            {
                public:
                    using CSystemModelType              =   systemmodels::nlti::mimo::CDiscreteTimeSystemModel<T,NA,NB,NC>;
                    using CJacobianMatricesType         =   filter::nlti::mimo::IJacobianMatrices<T,NA,NB,NC>;
                    using CStatesType                   =   linalg::CMatrix<T,NB,1>;
                    using CControlType                    =   linalg::CMatrix<T,NA,1>;
                    using CObservationType                   =   linalg::CMatrix<T,NC,1>;
                    using CJMTransitionType             =   linalg::CMatrix<T,NB,NB>;
                    using CJMObservationType            =   linalg::CMatrix<T,NC,NB>;
                    using CObservationNoiseType         =   linalg::CMatrix<T,NC,NC>;
                    using CKalmanGainType               =   linalg::CMatrix<T,NC,NB>;
                    
                    /* Constructor */
                    CEKF(CSystemModelType&                          f_systemModel
                        ,CJacobianMatricesType&                     f_jbMatrices
                        ,const CJMTransitionType&                   f_Q
                        ,const CObservationNoiseType&               f_R);
                    /* Predict */
                    void predict    (const CControlType&         f_input);
                    /* Update */
                    void update     (const CControlType&         f_input
                                    ,const CObservationType&        f_measurement);
                protected:
                private:
                    CSystemModelType&                    m_systemModel;
                    CJacobianMatricesType&               m_jbMatrices;
                    //Predicted covariance matrix
                    CJMTransitionType                   m_covarianceMatrix;
                    //The covariance of the process noise
                    CJMTransitionType                   m_Q;
                    //The covariance of the observation noise
                    CObservationNoiseType               m_R;

            }; // class CEKF
        }; //namespace mimo
    }; //namespace nlti
}; // namespace filter
#include "kalmanfilters.hpp"


#endif //KALMAN_FILTERS_HPP
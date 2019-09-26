/**
 * @file kalmanfilters.tpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-09-26
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef KALMAN_FILTERS_TPP
#define KALMAN_FILTERS_TPP

#ifndef KALMAN_FILTERS_HPP
#error __FILE__ should only be included from kalmanfilters.hpp.
#endif // KALMAN_FILTERS_HPP


namespace filter{
    namespace lti{
        namespace mimo{
            
            /******************************************************************************/
            /** @brief  CKalmanFilter Class constructor.
             * 
             *  Implementation of a general Kalman filter for system with multiple input and multiple output.
             *
             *  @param f_stateTransitionModel       The state transition model
             *  @param f_controlInputModel          The control-input model 
             *  @param f_observationModel           The observation model
             *  @param f_covarianceProcessNoise     The covariance of the process noise
             *  @param f_observationNoiseCovariance The covariance of the observation noise
             */
            template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
            CKalmanFilter<T,NA,NB,NC>::CKalmanFilter(
                    const CStateTransModelType&             f_stateTransitionModel,
                    const CControInputModelType&            f_controlInputModel,
                    const CObservationModelType&            f_observationModel,
                    const CProcessCovarianceType&           f_covarianceProcessNoise,
                    const CObservationCovarianceType&       f_observationNoiseCovariance)
                : m_stateTransitionModel(f_stateTransitionModel)
                , m_controlInputModel(f_controlInputModel)
                , m_observationModel(f_observationModel)
                , m_covarianceProcessNoise(f_covarianceProcessNoise)
                , m_observationNoiseCovariance(f_observationNoiseCovariance)
            {
            }

            /** @brief  Filter function
             *
             * It requires the control  and the observation signals. The control signals are input applied signals on the system states. 
             * The observation signals are the measusered values by sensors, these signal will need to be filtered. The filter contains two phase: predict and update.
             * 
             * @param f_control         control signal
             * @param f_observation     observation signal, the meausered values by the sensors. 
             * @return  The filtered observation values. 
             */
            template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
            typename CKalmanFilter<T,NA,NB,NC>::CObservationType CKalmanFilter<T,NA,NB,NC>::operator()(const CControlType& f_control,const CObservationType& f_observation )
            {
                predict(f_control);
                return update(f_observation);
            }

            /** @brief  Filter function without control values.
             *
             * It requires only observation values, which will be filtered. This function could be used, when the system has not control input.
             * Futhermore the control-input model become irrelevant for the filter. The filter contains two phase: predict and update.
             * 
             * @param f_observation     observation signal, the meausered values by the sensors.
             * @return The filtered observation values.
             */
            template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
            typename CKalmanFilter<T,NA,NB,NC>::CObservationType CKalmanFilter<T,NA,NB,NC>::operator()(const CObservationType& f_observation)
            {
                predict();
                return update(f_observation);
            }

            /** @brief  Predict phase of the Kalman filter without control values 
             *
             * In this phase, the filter predicts the next state and error covariance of the system, based on only the state transition model. 
             * It calculates the priori state and priori error covariance.
             * 
             */
            template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
            void CKalmanFilter<T,NA,NB,NC>::predict()
            {
                m_state        = m_stateTransitionModel * m_state;
                m_processCovariance   = m_stateTransitionModel * m_processCovariance * transpose(m_stateTransitionModel) + m_covarianceProcessNoise;
            }

            /** @brief  Predict phase of the Kalman filter with control values 
             *
             * The filter predicts the next state and error covariance of the system, based on the state transition model and control-input model. 
             * It calculates the priori state and priori error covariance.
             * 
             * @param f_control    control values 
             * 
             */
            template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
            void CKalmanFilter<T,NA,NB,NC>::predict(const CControlType& f_control)
            {
                m_state        = m_stateTransitionModel * m_state + m_controlInputModel * f_control; 
                m_processCovariance   = m_stateTransitionModel * m_processCovariance * transpose(m_stateTransitionModel) + m_covarianceProcessNoise;
            }

            /** @brief  Update phase of the Kalman filter
             *
             * In this phase the filter corrects the predicted state and error covariance of the system based on the measured observation values using Kalman gain. 
             * 
             * @return filtered observation value.
             */
            template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
            typename CKalmanFilter<T,NA,NB,NC>::CObservationType CKalmanFilter<T,NA,NB,NC>::update(const CObservationType& f_observation)
            {
                linalg::CVector<T, NC> l_measurementResidual;
                l_measurementResidual   = f_observation - m_observationModel * m_state;
                linalg::CMatrix<T,NC,NA> l_residualCovariance;
                
                l_residualCovariance    = m_observationModel * m_processCovariance * transpose(m_observationModel) + m_observationNoiseCovariance;
                m_kalmanGain            = m_processCovariance * transpose(m_observationModel) * l_residualCovariance.inv();
                m_state                 = m_state + m_kalmanGain * l_measurementResidual;
                m_processCovariance = ( CStateTransModelType::eye() - m_kalmanGain * m_observationModel ) * m_processCovariance;
                linalg::CVector<T, NC> l_filteredMeasurements = m_observationModel * m_state;
                return l_filteredMeasurements;
            }
        }; // mimo
    }; // namespace lti

    namespace nlti{
        namespace mimo{
            /******************************************************************************/
            /** @brief  CEKF Class constructor
             *
             *  It's a general implementation of a extended Kalman filter for nonlinear time invariant systems. It requires an implementation of CDiscreteTimeSystemModel and interface IJacobianMatrices. 
             *  The CDiscreteTimeSystemModel, IJacobianMatrices  describe the behavior of a system. 
             *
             *  @param f_systemModel    the discrete system model
             *  @param f_jbMatrices     the jacobians of the state transition and observation matrices. 
             *  @param f_Q              the process error covariance 
             *  @param f_R              the observation error covariance 
             */
            template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
            filter::nlti::mimo::CEKF<T,NA,NB,NC>::CEKF(
                    CSystemModelType&                           f_systemModel
                    ,CJacobianMatricesType&                     f_jbMatrices
                    ,const CJMTransitionType&                   f_Q
                    ,const CObservationNoiseType&               f_R)
                :m_systemModel(f_systemModel)
                ,m_jbMatrices(f_jbMatrices)
                ,m_covarianceMatrix(linalg::CMatrix<T,NB,NB>::ones())
                ,m_Q(f_Q)
                ,m_R(f_R)
            {
            }

            /** @brief  Predict phase, where estimate the system states based the model. 
             *
             * @param f_control         control signal vector
             * 
             */
            template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
            void filter::nlti::mimo::CEKF<T,NA,NB,NC>::predict(const CControlType&   f_control)
            {
                //Previous updated state
                CStatesType l_prev_states=m_systemModel.getStates();
                CJMTransitionType l_JF=m_jbMatrices.getJMTransition(l_prev_states,f_control);
                //Predicted state estimate X_{k|k-1}
                CStatesType l_pred_states=m_systemModel.update(f_control);
                //Predicted covariance estimate
                m_covarianceMatrix=l_JF*m_covarianceMatrix*l_JF.transpose()+m_Q;
            }

            /** @brief   Update phase, where corects the states of the system model.
             *
             * It only applies the filter to corect the system's states, does not return the filtered observation.
             * 
             * @param f_control         control signal vector
             * @param f_observation     measured observation vector 
             * 
             */
            template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
            void filter::nlti::mimo::CEKF<T,NA,NB,NC>::update(const CControlType&   f_control
                                                            ,const CObservationType&  f_observation)
            {
                // Estimated system output
                CObservationType l_y_est=m_systemModel.calculateOutput(f_control);
                // Innovation or measurement residual
                CObservationType l_mes_res=f_observation-l_y_est;
                // Innovation (or residual) covariance
                CStatesType l_states=m_systemModel.getStates();
                CJMObservationType l_JH=m_jbMatrices.getJMObservation(f_control,l_states);
                CObservationNoiseType l_s=l_JH*m_covarianceMatrix*l_JH.transpose()+m_R;
                //Near-optimal Kalman gain
                CKalmanGainType l_K=m_covarianceMatrix*l_JH.transpose()*l_s.inv();
                //Updated state estimate
                CStatesType l_updated_states=l_states+l_K*l_mes_res;
                m_systemModel.setStates(l_updated_states);
                //Updated covariance estimate
                m_covarianceMatrix=(CJMTransitionType::eye()-l_K*l_JH)*m_covarianceMatrix;
            }    
        }; //namespace mimo
    }; //namespace nlti

}; //namespace filter


#endif KALMAN_FILTERS_TPP
/**
  ******************************************************************************
  * @file    Filter.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class declaration for the filter
  *          functionality.
  ******************************************************************************
 */

/* Include guard */
#ifndef FILTER_HPP
#define FILTER_HPP

#include <Linalg/linalg.h>
#include <SystemModels/systemmodels.hpp>

namespace filter
{
    /**
     * @brief The filter interface
     * 
     * @tparam T The type of the input and output signal
     */
    template<class T>
    class CFilterFunction
    {
    public:
        virtual T operator()(T&)=0;
    };

    namespace lti
    {
        namespace siso
        {
            /**
             * @brief Infinite impulse response (IIR) discrete-time filter.
             * 
             */
            template <class T, uint32_t NA, uint32_t NB>
            class CIIRFilter:public CFilterFunction<T>
            {
            public:
                /* Constructor */
                CIIRFilter(const linalg::CRowVector<T,NA>& f_A,const linalg::CRowVector<T,NB>& f_B);
                /* Operator */
                T operator()(T& f_u);
            private:
                CIIRFilter(){}
                linalg::CRowVector<T,NA> m_A;
                linalg::CRowVector<T,NB> m_B;
                linalg::CColVector<T,NA> m_Y;
                linalg::CColVector<T,NB> m_U;
            };

            /**
             * @brief Finite impulse response (FIR) discrete-time filter.
             * 
             */
            template <class T, uint32_t NB>
            class CFIRFilter:public CFilterFunction<T>
            {
            public:
                /* Constructor */
                CFIRFilter(const linalg::CRowVector<T,NB>& f_B);
                /* Operator */
                T operator()(T& f_u);
            private:
                CFIRFilter() {}
                linalg::CRowVector<T,NB> m_B;
                linalg::CColVector<T,NB> m_U;
            };

            /**
             * @brief Mean filter
             * 
             * @tparam T 
             * @tparam NB 
             */
            template <class T, uint32_t NB>
            class CMeanFilter:public CFilterFunction<T>
            {
            public:
                /* Constructor */
                CMeanFilter();
                /* Operator */
                virtual T operator()(T& f_u);
            private:
                T m_B;
                linalg::CColVector<T,NB> m_U;
            };
        };

        namespace mimo
        {
            //!  CSSModel class. 
            /*!
             *  
             * */
            template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
            class CSSModel
            {
            public:
                using CStateType = linalg::CColVector<T,NA>;
                using CStateTransitionType = linalg::CMatrix<T,NA,NA>;
                using CInputType = linalg::CColVector<T,NB>;
                using CMeasurementType = linalg::CColVector<T,NC>;
                using CInputMatrixType = linalg::CMatrix<T,NA,NB>;
                using CMeasurementMatrixType = linalg::CMatrix<T,NC,NA>;
                using CDirectTransferMatrixType = linalg::CMatrix<T,NC,NB>;
                /* Constructor */
                CSSModel(
                    const CStateTransitionType& f_stateTransitionMatrix,
                    const CInputMatrixType& f_inputMatrix,
                    const CMeasurementMatrixType& f_measurementMatrix);
                /* Constructor */
                CSSModel(
                    const CStateTransitionType& f_stateTransitionMatrix,
                    const CInputMatrixType& f_inputMatrix,
                    const CMeasurementMatrixType& f_measurementMatrix,
                    const CDirectTransferMatrixType& f_directTransferMatrix);
                /* Constructor */
                CSSModel(
                    const CStateTransitionType& f_stateTransitionMatrix,
                    const CInputMatrixType& f_inputMatrix,
                    const CMeasurementMatrixType& f_measurementMatrix,
                    const CDirectTransferMatrixType& f_directTransferMatrix,
                    const CStateType& f_state);

                /* Get state method */
                const CStateType& state() const {return m_stateVector;} 
                CStateType& state() {return m_stateVector;} 

                /* Operator */
                CMeasurementType operator()(const CInputType& f_inputVector);
                /* Update state */
                void updateState(const CInputType& f_inputVector);
                /* Get output */
                CMeasurementType getOutput(const CInputType& f_inputVector);

            private:
                CSSModel() {}
                /* state vector */
                CStateType m_stateVector;
                /* state transition matrix */
                CStateTransitionType m_stateTransitionMatrix;
                /* input matrix */
                CInputMatrixType m_inputMatrix;
                /* measurement matrix */
                CMeasurementMatrixType m_measurementMatrix;
                /* direct transfer matrix */
                CDirectTransferMatrixType m_directTransferMatrix;
            };

            //!  CKalmanFilter class. 
            /*!
             *  
             * */
            template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
            class CKalmanFilter
            {
            public:
                using CStateType                 = linalg::CVector<T, NA>;
                using CInputVectorType           = linalg::CVector<T, NB>;
                using COutputVectorType          = linalg::CVector<T, NC>;
                using CInputType                 = linalg::CMatrix<T, NA, NB>;
                using CControInputType           = linalg::CMatrix<T, NA, NC>;
                using CModelCovarianceType       = linalg::CMatrix<T, NA, NA>;
                using CMeasurementCovarianceType = linalg::CMatrix<T, NC, NC>;
                using CStateTransType            = linalg::CMatrix<T, NA, NA>;
                using CMeasurementType           = linalg::CMatrix<T, NC, NA>;
                using CKalmanGainType            = linalg::CMatrix<T, NA, NC>;

                /* Constructor */
                CKalmanFilter(
                    const CStateTransType&            f_stateTransitionModel,
                    const CControInputType&           f_controlInput,
                    const CMeasurementType&           f_observationModel,
                    const CModelCovarianceType&       f_covarianceProcessNoise,
                    const CMeasurementCovarianceType& f_observationNoiseCovariance);

                /* Get state method */
                const CStateType& state() const
                {
                    return m_posterioriState;
                }
                /* Get state method */
                CStateType& state()
                {
                    return m_posterioriState;
                }
                /* Operator */
                CMeasurementType operator()(const CInputVectorType& f_input);
                /* Operator */
                CMeasurementType operator()();
                /* Predict */
                void predict();
                /* Predict */
                void predict(const CInputVectorType& f_input);
                /* Update */
                const CMeasurementType& update(void);

                private:
                CKalmanFilter() {}

                CStateType                 m_previousState;              // previous state
                CStateType                 m_prioriState;                // priori state
                CStateType                 m_posterioriState;            // posteriori state
                CControInputType           m_controlInput;               // control input modelþ
                CModelCovarianceType       m_previousCovariance;         // previous covariance estimate         // <-
                CModelCovarianceType       m_prioriCovariance;           // priori covariance estimate           // <-
                CModelCovarianceType       m_posterioriCovariance;       // posteriori covariance estimate       // <-
                CStateTransType            m_stateTransitionModel;       // state transition model
                CModelCovarianceType       m_covarianceProcessNoise;     // covariance of process noise          // done
                CMeasurementType           m_measurementResidual;        // innovation or measurement residual
                CMeasurementType           m_measurement;                // observation (or measurement)
                CMeasurementType           m_observationModel;           // observation model
                CMeasurementCovarianceType m_residualCovariance;         // innovation or residual covariance    // <-
                CMeasurementCovarianceType m_observationNoiseCovariance; // covariance of observation noise      // <-
                CKalmanGainType            m_kalmanGain;                 // optimal kalman gain
            };

        };
    };
    namespace ltv
    {
        namespace mimo
        {
            // Extended Kalman Filter
            // Template parameters:
            //              - T     variable type
            //              - NA    no. of inputs
            //              - NB    no. of states
            //              - NC    no. of outputs
            // Input parameters
            template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
            class CEKF
            {
                public:
                    using CSystemModelType              =   systemmodels::nlti::mimo::CDiscreteTimeSystemModel<T,NA,NB,NC>;
                    using CJacobianMatricesType         =   systemmodels::nlti::mimo::CJacobianMatrices<T,NA,NB,NC>;
                    using CStatesType                   =   linalg::CMatrix<T,NB,1>;
                    using CInputType                    =   linalg::CMatrix<T,NA,1>;
                    using COutputType                   =   linalg::CMatrix<T,NC,1>;
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
                    void predict    (const CInputType&         f_input);
                    /* Update */
                    void update     (const CInputType&         f_input
                                    ,const COutputType&        f_measurement);
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

            };
        };
    };

    namespace nonlinear
    {
        namespace siso
        {   
            
            /**
             * @brief Median filter
             * 
             */
            template <class T, uint32_t N>
            class CMedianFilter:public CFilterFunction<T>
            {
            public:
                /* Constructor */
                CMedianFilter();
                inline T addNewValue(T& f_val);
                inline T getMedian();
                inline T operator()(T& f_v);

            private:
                struct my_structure
                {
                    T info;
                      my_structure *next;
                      my_structure *prev;
                    
                };

                my_structure *m_median;    //pointer to the loist element that represents the median
                my_structure *m_smallest;  //pionter to the smallest element of the list (the first)
                uint32_t m_new;		     //index to tyhe newest element in the list
                uint32_t m_size;		     //the size of the list/queue

                my_structure m_queue[N];
            };
        }
    }
};

#include "filter.inl"

#endif // FILTER_H

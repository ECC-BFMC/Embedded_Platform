/**
  ******************************************************************************
  * @file    Filter.inl
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class implementations for the filter
  *          functionality.
  ******************************************************************************
 */

template <class T, uint32_t NC>
using CMeasurementType = linalg::CColVector<T,NC>;

/******************************************************************************/
/** @brief  CIIRFilter Class constructor
 *
 *  Constructor method
 *
 *  @param f_A   the feedback filter coefficients 
 *  @param f_B   the feedforward filter coefficients
 */
template <class T, uint32_t NA, uint32_t NB>
filter::lti::siso::CIIRFilter<T,NA,NB>::CIIRFilter(const linalg::CRowVector<T,NA>& f_A,const linalg::CRowVector<T,NB>& f_B) 
    : m_A(f_A)
    , m_B(f_B)
    , m_Y()
    , m_U() 
{
}

/** @brief  Operator
  *
  * @param f_u                 reference to input data
  * @return                        the filtered output data
  */
template <class T, uint32_t NA, uint32_t NB>
T filter::lti::siso::CIIRFilter<T,NA,NB>::operator()(T& f_u)
{
    for (uint32_t l_idx = NB-1; l_idx > 0; --l_idx)
    {
        m_U[l_idx] = m_U[l_idx-1];
    }
    m_U[0][0] = f_u;

    linalg::CMatrix<T,1,1> l_y = m_B*m_U - m_A*m_Y;
    // T l_y = m_B*m_U - m_A*m_Y;

    for (uint32_t l_idx = NA-1; l_idx > 0 ; --l_idx)
    {
        m_Y[l_idx] = m_Y[l_idx-1];
    }
    m_Y[0][0] = l_y[0][0];

    return m_Y[0][0];
}

/******************************************************************************/
/** @brief  CFIRFilter Class constructor
 *
 *  Constructor method
 *
 *  @param f_B   the feedforward filter coefficients
 */
template <class T, uint32_t NB>
filter::lti::siso::CFIRFilter<T,NB>::CFIRFilter(const linalg::CRowVector<T,NB>& f_B) 
    : m_B(f_B), m_U() 
{
}

/** @brief  Operator
  *
  * @param f_u                 reference to the input data
  * @return                        the filtered output data
  */
template <class T, uint32_t NB>
T filter::lti::siso::CFIRFilter<T,NB>::operator()(T& f_u)
{
    for (uint32_t l_idx = NB-1; l_idx > 0; --l_idx)
    {
        m_U[l_idx] = m_U[l_idx-1];
    }
    m_U[0] = f_u;

    linalg::CMatrix<T,1,1> l_y = m_B*m_U;

    return l_y[0][0];
}

/******************************************************************************/
/** @brief  MeanFilter Class constructor
 *
 *  Constructor method
 *
 *  
 */
template <class T, uint32_t NB> 
filter::lti::siso::CMeanFilter<T,NB>::CMeanFilter() 
    : m_B(1./NB)
    , m_U() 
{
}

/** @brief  Operator
  *
  * @param f_u                 reference to the input data
  * @return                        the filtered output data 
  */
template <class T, uint32_t NB> 
T filter::lti::siso::CMeanFilter<T,NB>::operator()(T& f_u)
{
    T l_y =0;

    for (uint32_t l_idx = 1; l_idx < NB; ++l_idx)
    {
        m_U[l_idx][0] = m_U[l_idx-1][0];
        l_y += m_U[l_idx][0];
    }
    m_U[0][0] = f_u;
    l_y += m_U[0][0];

    return m_B*l_y;
}

/******************************************************************************/
/** @brief  CSSModel Class constructor
 *
 *  Constructor method
 *
 *  @param f_stateTransitionMatrix
 *  @param f_inputMatrix
 *  @param f_measurementMatrix
 */
template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
filter::lti::mimo::CSSModel<T,NA,NB,NC>::CSSModel(
        const CStateTransitionType& f_stateTransitionMatrix,
        const CInputMatrixType& f_inputMatrix,
        const CMeasurementMatrixType& f_measurementMatrix) 
    : m_stateVector()
    , m_stateTransitionMatrix(f_stateTransitionMatrix)
    , m_inputMatrix(f_inputMatrix)
    , m_measurementMatrix(f_measurementMatrix)
    , m_directTransferMatrix()
{
    // do nothing
}

/** @brief  CSSModel Class constructor
 *
 *  Constructor method
 *
 *  @param f_stateTransitionMatrix
 *  @param f_inputMatrix
 *  @param f_measurementMatrix
 *  @param f_directTransferMatrix
 */
template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
filter::lti::mimo::CSSModel<T,NA,NB,NC>::CSSModel(
        const CStateTransitionType& f_stateTransitionMatrix,
        const CInputMatrixType& f_inputMatrix,
        const CMeasurementMatrixType& f_measurementMatrix,
        const CDirectTransferMatrixType& f_directTransferMatrix) 
    : m_stateVector()
    , m_stateTransitionMatrix(f_stateTransitionMatrix)
    , m_inputMatrix(f_inputMatrix)
    , m_measurementMatrix(f_measurementMatrix)
    , m_directTransferMatrix(f_directTransferMatrix)
{
    // do nothing
}

/** @brief  CSSModel Class constructor
 *
 *  Constructor method
 *
 *  @param f_stateTransitionMatrix
 *  @param f_inputMatrix
 *  @param f_measurementMatrix
 *  @param f_directTransferMatrix
 *  @param f_state
 */
template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
filter::lti::mimo::CSSModel<T,NA,NB,NC>::CSSModel(
        const CStateTransitionType& f_stateTransitionMatrix,
        const CInputMatrixType& f_inputMatrix,
        const CMeasurementMatrixType& f_measurementMatrix,
        const CDirectTransferMatrixType& f_directTransferMatrix,
        const CStateType& f_state) 
    : m_stateVector(f_state)
    , m_stateTransitionMatrix(f_stateTransitionMatrix)
    , m_inputMatrix(f_inputMatrix)
    , m_measurementMatrix(f_measurementMatrix)
    , m_directTransferMatrix(f_directTransferMatrix)
{
    // do nothing
}

/** @brief  Operator
  *
  * @param f_inputVector        reference to input vector
  * @return                         the output data from the system model (output vector)
  */
template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
CMeasurementType<T,NC> filter::lti::mimo::CSSModel<T,NA,NB,NC>::operator()(const CInputType& f_inputVector)
{
    updateState(f_inputVector);

    return getOutput(f_inputVector);
}

/** @brief  Update state method
  *
  * @param f_inputVector        reference to input vector
  * @return                         None
  */
template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
void filter::lti::mimo::CSSModel<T,NA,NB,NC>::updateState(const CInputType& f_inputVector)
{
    m_stateVector = m_stateTransitionMatrix * m_stateVector + m_inputMatrix * f_inputVector;
}

/** @brief  Get output
  *
  * @param f_inputVector        reference to input vector
  * @return                         the output data from the system model (output vector)
  */
template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
CMeasurementType<T,NC> filter::lti::mimo::CSSModel<T,NA,NB,NC>::getOutput(const CInputType& f_inputVector)
{
    return m_measurementMatrix * m_stateVector + m_directTransferMatrix * f_inputVector;
}

/******************************************************************************/
/** @brief  CKalmanFilter Class constructor
 *
 *  Constructor method
 *
 *  @param f_stateTransitionModel
 *  @param f_controlInput
 *  @param f_observationModel
 *  @param f_covarianceProcessNoise
 *  @param f_observationNoiseCovariance
 */
template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
filter::lti::mimo::CKalmanFilter<T,NA,NB,NC>::CKalmanFilter(
        const CStateTransType&            f_stateTransitionModel,
        const CControInputType&           f_controlInput,
        const CMeasurementType&           f_observationModel,
        const CModelCovarianceType&       f_covarianceProcessNoise,
        const CMeasurementCovarianceType& f_observationNoiseCovariance)
    : m_stateTransitionModel(f_stateTransitionModel)
    , m_controlInput(f_controlInput)
    , m_observationModel(f_observationModel)
    , m_covarianceProcessNoise(f_covarianceProcessNoise)
    , m_observationNoiseCovariance(f_observationNoiseCovariance)
{
}

/** @brief  Operator
  *
  * @param f_input              reference to input vector
  * @return                         Updated (a posteriori) state estimate
  */
template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
linalg::CMatrix<T, NC, NA> filter::lti::mimo::CKalmanFilter<T,NA,NB,NC>::operator()(const CInputVectorType& f_input)
{
    predict(f_input);
    return update();
}

/** @brief  Operator
  *
  * 
  * @return     Updated (a posteriori) state estimate
  */
template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
linalg::CMatrix<T, NC, NA> filter::lti::mimo::CKalmanFilter<T,NA,NB,NC>::operator()()
{
    predict();
    return update();
}

/** @brief  Predict
  *
  * 
  * 
  */
template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
void filter::lti::mimo::CKalmanFilter<T,NA,NB,NC>::predict()
{
    m_previousState      = m_prioriState;
    m_previousCovariance = m_prioriCovariance;
    m_prioriState        = m_stateTransitionModel * m_previousState;
    m_prioriCovariance   = m_stateTransitionModel * m_previousCovariance * transpose(m_stateTransitionModel) + m_covarianceProcessNoise;
}

/** @brief  Predict
  *
  * @param f_input    vector input
  * 
  */
template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
void filter::lti::mimo::CKalmanFilter<T,NA,NB,NC>::predict(const CInputVectorType& f_input)
{
    m_previousState      = m_prioriState;
    m_previousCovariance = m_prioriCovariance;
    m_prioriState        = m_stateTransitionModel * m_previousState + m_controlInput * f_input;
    m_prioriCovariance   = m_stateTransitionModel * m_previousCovariance * transpose(m_stateTransitionModel) + m_covarianceProcessNoise;
}

/** @brief  Update
  *
  * 
  * @return    Measurement residual
  */
template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
const linalg::CMatrix<T, NC, NA>& filter::lti::mimo::CKalmanFilter<T,NA,NB,NC>::update(void)
{
    m_measurementResidual  = m_measurement - m_observationModel * m_prioriState;
    m_measurement          = m_observationModel * m_posterioriState;
    m_residualCovariance   = m_observationModel * m_prioriCovariance * transpose(m_observationModel) + m_observationNoiseCovariance;
    m_kalmanGain           = m_prioriCovariance * transpose(m_observationModel) * m_residualCovariance.inv();
    m_posterioriState      = m_prioriState + m_kalmanGain * m_measurementResidual;
    m_posterioriCovariance = ( CStateTransType::eye() - m_kalmanGain * m_observationModel ) * m_prioriCovariance;
    return m_posterioriState;
}

/******************************************************************************/
/** @brief  CEKF Class constructor
 *
 *  Constructor method
 *
 *  @param f_systemModel
 *  @param f_jbMatrices
 *  @param f_Q
 *  @param f_R
 */
template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
filter::ltv::mimo::CEKF<T,NA,NB,NC>::CEKF(
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

/** @brief  Predict
  *
  * @param f_input    vector input
  * 
  */
template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
void filter::ltv::mimo::CEKF<T,NA,NB,NC>::predict(const CInputType&   f_input)
{
    //Previous updated state
    CStatesType l_prev_states=m_systemModel.getStates();
    CJMTransitionType l_JF=m_jbMatrices.getJMTransition(l_prev_states,f_input);
    //Predicted state estimate X_{k|k-1}
    CStatesType l_pred_states=m_systemModel.update(f_input);
    //Predicted covariance estimate
    m_covarianceMatrix=l_JF*m_covarianceMatrix*l_JF.transpose()+m_Q;
}

/** @brief  Update
  *
  * @param f_input          vector input
  * @param f_measurement    vector input
  * 
  */
template <class T, uint32_t NA, uint32_t NB, uint32_t NC>
void filter::ltv::mimo::CEKF<T,NA,NB,NC>::update(const CInputType&   f_input
                                                ,const COutputType&  f_measurement)
{
    // Estimated system output
    COutputType l_y_est=m_systemModel.calculateOutput(f_input);
    // Innovation or measurement residual
    COutputType l_mes_res=f_measurement-l_y_est;
    // Innovation (or residual) covariance
    CStatesType l_states=m_systemModel.getStates();
    CJMObservationType l_JH=m_jbMatrices.getJMObservation(f_input,l_states);
    CObservationNoiseType l_s=l_JH*m_covarianceMatrix*l_JH.transpose()+m_R;
    //Near-optimal Kalman gain
    CKalmanGainType l_K=m_covarianceMatrix*l_JH.transpose()*l_s.inv();
    //Updated state estimate
    CStatesType l_updated_states=l_states+l_K*l_mes_res;
    m_systemModel.setStates(l_updated_states);
    //Updated covariance estimate
    m_covarianceMatrix=(CJMTransitionType::eye()-l_K*l_JH)*m_covarianceMatrix;
}

/******************************************************************************/
/** @brief  CMedianFilter Class constructor
 *
 *  Constructor method
 *
 *  
 */
template <class T, uint32_t N>
filter::nonlinear::siso::CMedianFilter<T,N>::CMedianFilter()
    :m_median()
    ,m_smallest()
    ,m_new(0)
    ,m_size(N)
    ,m_queue()
{
    // for (unsigned int l_idx = 0; l_idx < N; l_idx++)
    // {
    //     m_queue[l_idx] = new structura;
        
    // }

    for (unsigned int l_idx = 0; l_idx < N ; l_idx++)
    {
        m_queue[l_idx].info = 0;
        m_queue[l_idx].next = &(m_queue[(l_idx + 1) % N]);
        m_queue[l_idx].prev = &(m_queue[(N + l_idx - 1) % N]);
    }

    m_new = N - 1;
    m_size = N;
    m_median = &(m_queue[m_size / 2]);
    m_smallest =&(m_queue[0]);
}

/** @brief  Operator
  *
  * @param f_val            the input data
  * @return                     filted the data 
  */
template <class T, uint32_t N>
T filter::nonlinear::siso::CMedianFilter<T,N>::operator()(T& f_val)
{
    m_new = (m_new + 1) % m_size;		 //shift new index	 //inca e valoarea veche
    /* // varianta pentru a decide valoarea mediana eficient-----eficient daca filtrul are dimensiuni mari
    //	->V2 start	  remy_structurere          EXISTA CAUZE CARE NU SUNT TRATATE CORECT SAU NETRATATE COMPLET!!!!!!!!!!!
    if ((m_queue[m_new]->info > m_median->info) && (f_val <= m_median->info))
    {
        if (f_val > m_median->prev->info)
        {
            m_median = m_queue[m_new];		   //med=new
        }
        else
        {
            m_median = m_median->prev;		   // <-
        }
    }
    else if ((m_queue[m_new]->info < m_median->info) && (f_val > m_median->info))
    {
        if (f_val > m_median->next->info)
        {
            m_median = m_median->next;		  // ->
        }
        else
        {   
            m_median = m_queue[m_new];		  //med=new
        }
    }
    else if ((m_queue[m_new]->info == m_median->info))
    {
        if ((f_val < m_median->info)&&(f_val <= m_median->prev->info))
        {
            m_median = m_median->prev;		   // <-
        }
        else if (f_val > m_median->info)
            {
                if (f_val <= m_median->next->info)
                {
                    m_median = m_queue[m_new];		  //med=new
                }
                else
                {
                    m_median = m_median->next;		  // ->
                }
            }
            else
            {
                m_median = m_queue[m_new];		  //med=new
            }
    }
    */
    m_queue[m_new ].info = f_val;		 //suprascrie cea mai veche valoare

    //ordonare dupa valoare

        //elementul new se "scoate" din lista
    m_queue[m_new].prev->next = m_queue[m_new].next;			 //5.
    m_queue[m_new].next->prev = m_queue[m_new].prev;			 //6.

        //update smallest value
    my_structure* l_i = m_smallest;
    if (&(m_queue[m_new]) == m_smallest)
    {
        if (m_queue[m_new].info > m_smallest->next->info)
        {
            m_smallest = m_smallest->next;
            l_i = m_smallest;
        }
    }
    else if (m_queue[m_new].info <= m_smallest->info)
    {
        l_i = m_smallest;
        m_smallest = &m_queue[m_new];
    }

        //cautarea locului unde trebuie sa se amplaseze noul element in lista
    unsigned int l_cnt = 1;
    if (&(m_queue[m_new]) == l_i)
    {
        l_i = l_i->next;
    }

    while ((l_i->info < m_queue[m_new].info) && (l_cnt <= m_size - 1))
    {
        l_i = l_i->next;
        l_cnt++;
    }

        //inserarea elemntului new la locul potrivit
    l_i->prev->next = &m_queue[m_new];							 //1.
    m_queue[m_new].next = l_i;									 //2.
    m_queue[m_new].prev = l_i->prev;							 //3.
    l_i->prev = &m_queue[m_new];									 //4.

    //varianta ineficienta pentru aflarea medianului  cand filtrul are dimensiuni mari
    m_median=m_smallest;
    for(uint8_t iddx=0; iddx < m_size/2; ++iddx)
    {
        m_median=m_median->next;
    }

    return getMedian();
}

/*  */
/** @brief  OperGet medianator
  *
  * 
  * @return    median value
  */
template <class T, uint32_t N>
T filter::nonlinear::siso::CMedianFilter<T, N>::getMedian()
{
    T ret_val;
    if (1 == m_size % 2)		// daca filtrul are lungime impara
    {
        ret_val = m_median->info;
    }
    else						// daca filtrul are lungime para
    {
        ret_val = 0.5*(m_median->info + m_median->prev->info);
    }
    return ret_val;
}
/******************************************************************************/
/** \brief  CDiscreteTransferFucntion class constructor
 *
 *  Constructor method
 */
template <class T,uint32_t NNum,uint32_t NDen>
systemmodels::lti::siso::CDiscreteTransferFucntion<T,NNum,NDen>::CDiscreteTransferFucntion()
    :m_num(CNumType::zeros())
    ,m_den(CDenModType::zeros())
    ,m_denCoef(1)
    ,m_memInput()
    ,m_memOutput()
{
}


template <class T,uint32_t NNum,uint32_t NDen>
systemmodels::lti::siso::CDiscreteTransferFucntion<T,NNum,NDen>::CDiscreteTransferFucntion(const CNumType& f_num,const CDenType& f_den)
    :m_num(CNumType::zeros())
    ,m_den(CDenModType::zeros())
    ,m_denCoef(1)
    ,m_memInput()
    ,m_memOutput()
{
    this->setNum(f_num);
    this->setDen(f_den);
}


/** \brief  Clear memory
 *
 *  
 *  
 */
template <class T,uint32_t NNum,uint32_t NDen>
void systemmodels::lti::siso::CDiscreteTransferFucntion<T,NNum,NDen>::clearMemmory()
{
    m_memInput=CInputMem::zeros();
    m_memOutput=COutputMem::zeros();
}

/** \brief  Clear memory
 *
 *  @param f_input      reference to input
 *  @return    l_output
 */
template <class T,uint32_t NNum,uint32_t NDen>
T systemmodels::lti::siso::CDiscreteTransferFucntion<T,NNum,NDen>::operator()(const T& f_input)
{
    shiftMemory<NNum>(m_memInput);
    m_memInput[0][0]=f_input;
    T l_output=((m_memInput*m_num-m_memOutput*m_den)[0][0])/m_denCoef;
    shiftMemory<NDen-1>(m_memOutput);
    m_memOutput[0][0]=l_output;
    return l_output;
}

/** \brief  Set num 
 *
 *  @param f_num      reference to nominator
 *  
 */
template <class T,uint32_t NNum,uint32_t NDen>
void systemmodels::lti::siso::CDiscreteTransferFucntion<T,NNum,NDen>::setNum(const CNumType& f_num)
{
    for(uint32_t i=0;i<NNum;++i)
    {
        m_num[i][0]=f_num[i][0];
    }
}

/** \brief  Set den 
 *
 *  @param f_den      reference to denominator
 *  
 */
template <class T,uint32_t NNum,uint32_t NDen>
void systemmodels::lti::siso::CDiscreteTransferFucntion<T,NNum,NDen>::setDen(const CDenType& f_den)
{
    for(uint32_t i=1;i<NDen;++i)
    {
        m_den[i-1][0]=f_den[i][0];
    }
    m_denCoef = f_den[0][0];
}

/** \brief  Get output 
 *
 *  
 *  @return    Output memory
 */
template <class T,uint32_t NNum,uint32_t NDen>
T systemmodels::lti::siso::CDiscreteTransferFucntion<T,NNum,NDen>::getOutput()
{
    return m_memOutput[0][0];
}

/** \brief  Shift memory
 *
 *  @param f_mem           reference to memory matrix
 *  @return    none
 */
template <class T,uint32_t NNum,uint32_t NDen>
template<uint32_t N>
void systemmodels::lti::siso::CDiscreteTransferFucntion<T,NNum,NDen>::shiftMemory(linalg::CMatrix<T,1,N>& f_mem)
{
    for(uint32_t i=N-1;i>0;--i)
    {
        f_mem[0][i]=f_mem[0][i-1];
    }
}

/******************************************************************************/
/** \brief  CDiscreteTimeSystemModel class constructor
 *
 *  Constructor method
 *
 *  @param f_dt      Time step
 */
template <class T,uint32_t NA, uint32_t NB,uint32_t NC>
systemmodels::nlti::mimo::CDiscreteTimeSystemModel<T,NA,NB,NC>::CDiscreteTimeSystemModel(const double f_dt)
    : m_states()
    , m_dt(f_dt)
{
}

/** \brief  CDiscreteTimeSystemModel class constructor
 *
 *  Constructor method
 *
 *  @param f_states  States
 *  @param f_dt      Time step
 */
template <class T,uint32_t NA, uint32_t NB,uint32_t NC>
systemmodels::nlti::mimo::CDiscreteTimeSystemModel<T,NA,NB,NC>::CDiscreteTimeSystemModel(
        const CStatesType&     f_states
       ,const double           f_dt)
    : m_states(f_states)
    , m_dt(f_dt)
{
}



/** \brief  Getter numitor 
 *
 *  @return    const reference to numitor
 */
template <class T,uint32_t NNum,uint32_t NDen>
const typename systemmodels::lti::siso::CDiscreteTransferFucntion<T,NNum,NDen>::CNumType& systemmodels::lti::siso::CDiscreteTransferFucntion<T,NNum,NDen>::getNum(){
    return m_num;
}

/** \brief  Getter denomitor
 * Return the denomitor without the first coefficeint.
 * 
 *  @return    const reference to denomitor
 */
template <class T,uint32_t NNum,uint32_t NDen>
const typename systemmodels::lti::siso::CDiscreteTransferFucntion<T,NNum,NDen>::CDenModType& systemmodels::lti::siso::CDiscreteTransferFucntion<T,NNum,NDen>::getDen(){
    return m_den;
}  

/** \brief  Getter denomitor first coefficeint
 *
 *  @return    value of the denomitor first coefficeint
 */
template <class T,uint32_t NNum,uint32_t NDen>
float systemmodels::lti::siso::CDiscreteTransferFucntion<T,NNum,NDen>::getDenCurrent(){
    return m_denCoef;
}
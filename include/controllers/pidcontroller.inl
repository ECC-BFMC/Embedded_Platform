/** @brief  Class constructor
  *
  * Constructor method
  *
  * @param f_kp                proportional factor
  * @param f_ki                integral factor
  * @param f_kd                derivative factor
  * @param f_tf                derivative time filter constant
  * @param f_dt                sample time
  */
template<class T>
CPidController<T>::CPidController(T              f_kp
                                                   ,T              f_ki
                                                   ,T              f_kd
                                                   ,T              f_tf
                                                   ,T              f_dt)
    :m_pidTf()
    ,m_dt(f_dt)
{    
    linalg::CMatrix<T,1,3> l_numPid({ (f_kd+f_tf*f_kp)/f_tf , (f_tf*f_dt*f_ki+f_dt*f_kp-2*f_tf*f_kp-2*f_kd)/f_tf , (f_kd+f_tf*f_kp+f_dt*f_dt*f_ki-f_dt*f_tf*f_ki-f_dt*f_kp)/f_tf});
    linalg::CMatrix<T,1,3> l_denPid({ 1.0,-(2*f_tf-f_dt)/f_tf,(f_tf-f_dt)/f_tf });

    m_pidTf.setNum(l_numPid.transpose());
    m_pidTf.setDen(l_denPid.transpose());
}

// template<class T>
// CPidController<T>::CPidController(  CPidSystemmodelType             f_pid
//                                     ,T                              f_dt)
//                                     :m_pidTf(f_pid)
//                                     ,m_dt(f_dt){

// }


/** @brief  Operator
  *
  * @param f_input             input function
  * \return                    control value
  */
template<class T>
T CPidController<T>::calculateControl(const T& f_input)
{
    return m_pidTf(f_input);
}

/** @brief  Serial callback method
  *
  * Serial callback attaching serial callback to controller object
  *
  * @param  obj                 PID controller object
  * @param  a                   string to read data from
  * @param b                    string to write data to
  * \return None
  */
template<class T>
void CPidController<T>::staticSerialCallback(void* obj,char const * a, char * b)
{
    CPidController* self = static_cast<CPidController*>(obj);
    self->serialCallback(a,b);
}

/** @brief  Reset to zero all memory of the controller.
  *
  */
template<class T>
void CPidController<T>::clear()
{
    m_pidTf.clearMemmory();
}

/** @brief  Set the parameter of the controller
  *
  *
  * @param f_kp                proportional factor
  * @param f_ki                integral factor
  * @param f_kd                derivative factor
  * @param f_tf                derivative time filter constant
  */
template<class T>
void CPidController<T>::setController(
    T              f_kp,
    T              f_ki,
    T              f_kd,
    T              f_tf)
{
    linalg::CMatrix<T,1,3> l_numPid({ (f_kd+f_tf*f_kp)/f_tf , (f_tf*m_dt*f_ki+m_dt*f_kp-2*f_tf*f_kp-2*f_kd)/f_tf , (f_kd+f_tf*f_kp+m_dt*m_dt*f_ki-m_dt*f_tf*f_ki-m_dt*f_kp)/f_tf });
    linalg::CMatrix<T,1,3> l_denPid({ 1,-(2*f_tf-m_dt)/f_tf,(f_tf-m_dt)/f_tf });
    
    m_pidTf.setNum(l_numPid.transpose());
    m_pidTf.setDen(l_denPid.transpose());
}

/** @brief  Serial callback method setting controller to values received
  *
  *
  * @param  a                   string to read data from
  * @param b                    string to write data to
  */
template<class T>
void CPidController<T>::serialCallback(char const * a, char * b)
{
    float l_kp,l_ki,l_kd,l_tf;
    uint32_t l_res = sscanf(a,"%f;%f;%f;%f;",&l_kp,&l_ki,&l_kd,&l_tf);
    if (4 == l_res)
    {
        setController(l_kp,l_ki,l_kd,l_tf);
        sprintf(b,"ack;;%2.5f;%2.5f;%2.5f;%2.5f;",l_kp,l_ki,l_kd,l_tf);
    }
    else
    {
        sprintf(b,"sintax error;;");
    }
}
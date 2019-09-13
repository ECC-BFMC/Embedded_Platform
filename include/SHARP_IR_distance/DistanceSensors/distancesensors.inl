/** @brief  Class constructor


    @param f_period       sensor reading period
    @param f_baseTick     base tick period, in seconds
    @param f_sensor_list  the list of sensor objects
 */
template<class T >
CDistanceSensors<T>::CDistanceSensors(const  uint32_t f_period,
                            const float f_baseTick,
                            T& f_sensor_list)
    : task::CTask(f_period)
    , m_sensor_list(f_sensor_list)
    , m_index(0)
    , m_N(f_sensor_list.size())
    , m_disableTimeOut()
    , m_enableTimeOut()
    , m_message("@IRDS:")
    // , m_buf("")
{
    //static_assert(f_sensor_list.size() * 0.03 < f_period*f_baseTick,
        //"IR sensor reding period is too small! It must be grater than: number of sensors in \"conflict\" *0.3seconds!");
}

//static_assert(m_N * 0.03 < f_period*f_baseTick,
//        "IR sensor reding period is too small! It must be grater than: number of sensors in \"conflict\" *0.3seconds!");

/** @brief  Class destructor

    Destructor method
 */
template<class T >
CDistanceSensors<T>::~CDistanceSensors(){}

/** @brief  Read callback method

    Callback for reading the value of one distance sensor

 */
template<class  T >
void CDistanceSensors<T>::ReadCallback(){
    // // buffer for holding message data
    // char buf [10];
    // read distance form sensor at position m_index in the sensor list
    m_sensor_list[m_index]->ReadDistance();
    // disable sensor at position m_index in the sensor list
    m_sensor_list[m_index]->disable();
    //sprintf(buf,"-%d-%3.2f;",m_index, m_sensor_list[m_index]->getValue());/// cu m_index
    // format value so it can be appended to message
    // sprintf(buf,"%3.2f;", m_sensor_list[m_index]->getValue()); // standard formatting
    // // append value from sensor to message
    // m_message.append(buf);
    // if the last sensor is read, send message on UART and exit method
    if(m_index == m_N-1){
        // m_message=m_message+";\r\n";
        // m_serial.printf(m_message.c_str());
        // m_message="@IRDS:";
        return;
    }
    // go to next sensor
    m_index = m_index+1;
    // enable next sensor
    m_sensor_list[m_index]->enable();
    // disable sensor timeout, read next sensor after 0.03 seconds
    m_disableTimeOut.attach(callback(this, &CDistanceSensors<T>::ReadCallback), 0.03);
}

/** @brief  Method called each f_period
 * 
 */
template<class T >
void CDistanceSensors<T>::_run(){
    // start from first sensor
    m_index=0;
    // enable first sensor
    m_sensor_list[m_index]->enable();
    // read sensor after 0.03 seconds
    m_disableTimeOut.attach(callback(this, &CDistanceSensors<T>::ReadCallback), 0.03);
}
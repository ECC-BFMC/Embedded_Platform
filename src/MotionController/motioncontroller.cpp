/**
  ******************************************************************************
  * @file    MotionController.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class definition for the motion controller
  *          functionality.
  ******************************************************************************
 */

#include <MotionController/motioncontroller.hpp>


/** \brief  CMotionController Class constructor
 *
 *  Constructor method
 *
 *  @param f_period_sec          period for controller execution in seconds
 *  @param f_serialPort          reference to serial communication object
 *  @param f_car                 reference to MOVE object
 *  @param f_safetyStop          reference to spline safery stop object
 *  @param f_control             reference to controller object
 */
CMotionController::CMotionController(
        float f_period_sec,
        Serial& f_serialPort,
        Move& f_car,
        CSafetyStopFunction* f_safetyStop,
        controllers::CControllerSiso* f_control) 
    : m_serialPort(f_serialPort)
    , m_car(f_car)
    , m_speed()
    , m_angle()
    , m_period_sec(f_period_sec)
    , m_isSplineActivated(false)
    , m_ispidActivated(false)
    , m_motionPlanner()
    , m_hbTimeOut()
    , m_control(f_control)
    , m_safetyStop(f_safetyStop)
    , m_timer(mbed::callback(CMotionController::staticCallbackRun,this))
{
}


/** \brief  CMotionController Class constructor
 *
 *  Constructor method
 *
 *  @param f_period_sec          period for controller execution in seconds
 *  @param f_serialPort          reference to serial communication object
 *  @param f_car                 reference to MOVE object
 *  @param f_control             reference to controller object
 */
CMotionController::CMotionController(
        float f_period_sec,
        Serial& f_serialPort,
        Move& f_car,
        controllers::CControllerSiso*           f_control) 
    : m_serialPort(f_serialPort)
    , m_car(f_car)
    , m_speed()
    , m_angle()
    , m_period_sec(f_period_sec)
    , m_isSplineActivated(false)
    , m_ispidActivated(false)
    , m_motionPlanner()
    , m_hbTimeOut()
    , m_control(f_control)
    , m_safetyStop(NULL)
    , m_timer(mbed::callback(CMotionController::staticCallbackRun,this))
{
}

/** \brief  Serial callback method
  *
  * Serial callback attaching serial callback to controller object
  *
  * @param  obj                 PID controller object
  * @param  a                   string to read data from
  * @param b                   string to write data to
  * 
  */
void CMotionController::staticSerialCallbackMove(void* obj,char const * a, char * b)
{
    CMotionController* self = static_cast<CMotionController*>(obj);
    self->serialCallbackMove(a,b);
}

/** \brief  Serial callback method for BRAKE command
  *
  * Serial callback attaching serial callback to controller object
  *
  * @param  obj                 PID controller object
  * @param  a                   string to read data from
  * @param b                   string to write data to
  * 
  */
void CMotionController::staticSerialCallbackBrake(void* obj,char const * a, char * b)
{
    CMotionController* self = static_cast<CMotionController*>(obj);
    self->serialCallbackBrake(a,b);
}

/** \brief  Serial callback method for hard BRAKE command
  *
  * Serial callback attaching serial callback to controller object
  *
  * @param  obj                 PID controller object
  * @param  a                   string to read data from
  * @param b                   string to write data to
  * 
  */
void CMotionController::staticSerialCallbackHardBrake(void* obj,char const * a, char * b) 
{
    CMotionController* self = static_cast<CMotionController*>(obj);
    self->serialCallbackHardBrake(a,b);
}

/** \brief  Serial callback method for PID activation command
  *
  * Serial callback attaching serial callback to controller object
  *
  * @param  obj                 PID controller object
  * @param  a                   string to read data from
  * @param b                   string to write data to
  * 
  */
void CMotionController::staticSerialCallbackPID(void* obj,char const * a, char * b) 
{
    CMotionController* self = static_cast<CMotionController*>(obj);
    self->serialCallbackPID(a,b);
}

/**
 * @brief Static seriel callback for spline command
 * 
 * @param obj                      object with the serial callback method
 * @param a                        string to read data from
 * @param b                        string to write data to
 */
void CMotionController::staticSerialCallbackSpline(void* obj, char const * a,char * b){
    CMotionController* self = static_cast<CMotionController*>(obj);
    self->serialCallbackSpline(a,b);
}

/** \brief  Reset method
 * 
 *  
 *  
 */
void CMotionController::reset()
{   
    m_speed = 0;
    m_angle = 0;
}

/** \brief  Get speed method
 * 
 *  
 *  \return     Speed
 */
float CMotionController::getSpeed() 
{
    return m_speed;
}

/** \brief  Get angle method
 * 
 *  
 *  \return     Angle
 */
float CMotionController::getAngle() 
{
    return m_angle;
}

/** \brief  BrakeCallback method
 * 
 *  
 *  
 */
void CMotionController::BrakeCallback(){
    m_state=2;
}

/** \brief  Set state method
 * 
 *  @param  f_state
 *  
 */
void CMotionController::setState(int f_state){
    m_state = f_state;
}

/** \brief  Method called each f_period
 * 
 *  
 *  
 */
void CMotionController::_run()
{
    if(m_isSplineActivated) 
    {
        if(m_motionPlanner.hasValidValue())
        {
            std::pair<float,float> motion=m_motionPlanner.getNextVelocity();  
            float l_dir=m_motionPlanner.getForward()?1:-1;
            if(m_ispidActivated)
            {
                m_speed=motion.first*l_dir;
                m_angle=-motion.second;
            }
            else
            {
                //Pid isn't activated. It have to controllered with the robot speed value in meter per second.  
                m_serialPort.printf("@SPLN:Err1;;\r\n");
                m_state=2;
                m_isSplineActivated=false;
                m_speed=0.0;
            }
        }
        else
        {
            m_serialPort.printf("@SPLN:Stop;;\r\n");
            m_speed=0.0;
            m_state=2;
            m_isSplineActivated=false;
        }
    }
    
    //safety stop function
    /**
     * @brief It's a part of safety stop functionilty. Check if it's actived or created.
     * 
     */
    if ((m_state!=2)&&(m_safetyStop!=NULL && m_safetyStop->isSafetyStopActive(m_speed,m_angle)==true))
    {
        m_state = 2;
    }

    switch(m_state)
    {
        // Move state
        case 1:
            m_car.Steer(m_angle);
            if(m_ispidActivated && m_control!=NULL)
            {
                m_control->setRef(CMotionController::Mps2Rps( m_speed ));
                // Calculate control signal
                int8_t l_isCorrect = m_control->control(); 
                // Check the state of the control method
                if( l_isCorrect == -1 )
                {
                    // In this case the encoder is working fine and measure too high speed, than it changes to the braking state.  
                    m_serialPort.printf("@PIDA:Too high speed and the encoder working;;\r\n");
                    m_car.Brake();
                    m_control->clear();
                    m_state = 2;
                }
                else if (l_isCorrect == -2 )
                {
                    // In this case the encoder fails and measures 0 rps, but the control signal had high values. 
                    // This part protects the robot to run with high speed, when the encoder doesn't measure correctly.
                    m_serialPort.printf("@PIDA:Encoder error;;\r\n");
                    m_car.Brake();
                    m_control->clear();
                    m_state = 2;
                }
                else
                {
                    m_car.Speed(m_control->get()*100.0);//Y
                }
                
            }
            else
            {
                m_car.Speed(m_speed);
            }
            break;
        // Brake state
        case 2:
            m_car.Steer(m_angle);
            m_car.Brake();
            if( m_control!=NULL){
                m_control->clear();
            }
            break;
    }
    
}

/** \brief  Serial callback actions for MOVE command
  *
  * Serial callback method setting controller to values received
  *
  * @param  a                   string to read data from
  * @param b                   string to write data to
  * 
  */
void CMotionController::serialCallbackMove(char const * a, char * b)
{
    float l_speed;
    float l_angle;
    uint32_t l_res = sscanf(a,"%f;%f",&l_speed,&l_angle);
    if (2 == l_res)
    {
        if( !m_ispidActivated && std::abs(l_speed) > MCTL_PWM_COMMAND_LIMIT ){
            sprintf(b,"Command is too high;;");
            return;
        }if( m_ispidActivated && std::abs(l_speed) > MCTL_SPEED_COMMAND_LIMIT ){
            sprintf(b,"Command is too high;;");
            return;
        }

        m_speed = l_speed;
        m_angle = l_angle; 
        m_isSplineActivated=false;
        m_state=1;
        sprintf(b,"ack;;");
    }
    else
    {
        sprintf(b,"sintax error;;");
    }
}

/** \brief  Serial callback actions for BRAKE command
  *
  * Serial callback method setting controller to values received
  *
  * @param  a                  string to read data from
  * @param b                   string to write data to
  * 
  */
void CMotionController::serialCallbackBrake(char const * a, char * b)
{
    float l_angle;
    uint32_t l_res = sscanf(a,"%f",&l_angle);
    if(1 == l_res)
    {
        m_speed = 0;
        m_angle = l_angle;
        // Brake state 
        m_state = 2;
        // Deactivate Spline (Bezier) planner 
        m_isSplineActivated=false;
        if( m_control!=NULL){
            m_control->setRef(0);
        }
        
        sprintf(b,"ack;;");           
    }
    else
    {
        sprintf(b,"sintax error;;");
    }
}

/** \brief  Serial callback actions for hard BRAKE command
  *
  * Serial callback method setting controller to values received
  *
  * @param  a                  string to read data from
  * @param b                   string to write data to
  * 
  */
void CMotionController::serialCallbackHardBrake(char const * a, char * b)
{
    float l_brake,l_angle;
    uint32_t l_res = sscanf(a,"%f;%f",&l_brake,&l_angle);
    if(2 == l_res && m_state!=0)
    {
        m_speed=0;
        m_angle = l_angle; 
        m_car.Inverse(l_brake);
        m_hbTimeOut.attach(callback(this,&CMotionController::BrakeCallback),0.04);
        m_state = 0;
        // Deactivate Spline (Bezier) planner 
        m_isSplineActivated=false;
        sprintf(b,"ack;;");           
    }
    else
    {
        sprintf(b,"sintax error;;");
    }
}

/** \brief  Serial callback actions for hard PID activation command
  *
  * Serial callback method setting controller to values received
  *
  * @param  a                   string to read data from
  * @param b                   string to write data to
  * 
  */
void CMotionController::serialCallbackPID(char const * a, char * b)
{
    int l_isActivate=0;
    uint32_t l_res = sscanf(a,"%d",&l_isActivate);
    if(l_res==1)
    {   
        if(m_control==NULL){
            sprintf(b,"Control object wans't instances. Cannot be activate pid controller;;");
        }else{
            m_speed = 0;
            m_ispidActivated=(l_isActivate>=1);
            // Change to brake state
            m_state = 2;
            // Deactivate Spline (Bezier) planner 
            m_isSplineActivated=false;
            sprintf(b,"ack;;");    
        }
        
    }else
    {
        sprintf(b,"sintax error;;");
    }
}

/**
 * @brief Serial callback actions for bezier spline command
 * 
 * @param a                   string to read data from
 * @param b                   string to write data to
 */
void CMotionController::serialCallbackSpline(char const * a, char * b){
    float a_x,a_y,b_x,b_y,c_x,c_y,d_x,d_y,duration_sec;
    int isForward=1;
    int32_t nrData=sscanf(a,"%d;%f;%f;%f;%f;%f;%f;%f;%f;%f",
                                    &isForward,
                                    &a_x,
                                    &a_y,
                                    &b_x,
                                    &b_y,
                                    &c_x,
                                    &c_y,
                                    &d_x,
                                    &d_y,
                                    &duration_sec);
    if(10==nrData && duration_sec>0 && (isForward==1 || isForward==0)){
        m_motionPlanner.setMotionPlannerParameters(static_cast<bool>(isForward), std::complex<float>(a_x,a_y),std::complex<float>(b_x,b_y),std::complex<float>(c_x,c_y),std::complex<float>(d_x,d_y),duration_sec,m_period_sec);
        m_isSplineActivated=true;
        m_state=1;
        sprintf(b,"ack;;");
    }else{
        sprintf(b,"sintax error;;");
    }
}


/**
 * @brief Function to convert from linear velocity ( centimeter per second ) of robot to angular velocity ( rotation per second ) of motor.
 * 
 * @param f_vel_mps linear velocity
 * @return float angular velocity
 */
float CMotionController::Mps2Rps(float f_vel_mps){
    return f_vel_mps * 150.0;
}

/**
 * @brief Static callback function for run method
 * 
 * @param obj pointer for the object
 */
void CMotionController::staticCallbackRun(void* obj){
    CMotionController* self = static_cast<CMotionController*>(obj);
    self->_run();
}  

/**
 * @brief Start RtosTimer, which periodically apply the run method. The period of the task is defined in the contructor.
 * 
 */
void CMotionController::startRtosTimer(){
    this->m_timer.start(static_cast<int>(m_period_sec*1000));
}
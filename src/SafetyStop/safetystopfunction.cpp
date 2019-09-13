/**
  ******************************************************************************
  * @file    SafetyStopFunction.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-2017
  * @brief   This file contains the class definition for the safety stop activation
  *          methods.
  ******************************************************************************
 */

#include <SafetyStop/safetystopfunction.hpp>


/**
 * @brief Construct a new CSafetyStopFunction::CSafetyStopFunction object
 * 
 * @param f_senLeftFront sensor mounteed on left front part
 * @param f_senMidLeftFront sensor mounteed on mid left front part
 * @param f_senCenterFront sensor mounted on middle front part
 * @param f_senMidRightFront sensor mounteed on mid right front part
 * @param f_senRightFront sensor mounteed on right front part
 */
CSafetyStopFunction::CSafetyStopFunction(
                      C_IR_DistanceSensorDriver& f_senLeftFront
                    , C_IR_DistanceSensorDriver& f_senMidLeftFront
                    , C_IR_DistanceSensorDriver& f_senCenterFront
                    , C_IR_DistanceSensorDriver& f_senMidRightFront
                    , C_IR_DistanceSensorDriver& f_senRightFront
                    // , C_IR_DistanceSensorDriver& f_senLeftBack
                    // , C_IR_DistanceSensorDriver& f_senRightBack
                    )
    : m_active(false) 
    ,m_speed(0)
    , m_angle(0)
    , m_senLeftFront(f_senLeftFront)
    , m_senMidLeftFront(f_senMidLeftFront)
    , m_senCenterFront(f_senCenterFront)
    , m_senMidRightFront(f_senMidRightFront)
    , m_senRightFront(f_senRightFront)
    // , m_senLeftBach(f_senLeftBack)
    // , m_senRightBach(f_senRightBack)
{
}

/** @brief  Class destructor
 *
 *  Destructor method
 */
CSafetyStopFunction::~CSafetyStopFunction()
{}

/** @brief  Function for activating safety stop
 * 
 *  @param  f_speed
 *  @param f_angle
 */
bool CSafetyStopFunction::isSafetyStopActive(float f_speed, float f_angle){
    // Verification the state of the functionality
    if(!this->m_active){
        // Inactive -> no need to brake
        return false;
    }
    // Get current speed
    m_speed = f_speed;
    // Get current angle
    m_angle = f_angle;
    // Value indicating whether brake should be activated
    bool ret_val  =false;
    // dyunamic threshold for activating brake
    float threshold;
    // If moving forward take into account only front sensors
    if (m_speed > 0)
    {
        threshold = m_speed*5-(1/m_speed)*10;
        if ((m_senLeftFront.getValue() <= threshold) ||
            (m_senMidLeftFront.getValue() <= threshold) ||
            (m_senCenterFront.getValue() <= threshold) || 
            (m_senMidRightFront.getValue() <= threshold) || 
            (m_senRightFront.getValue() <= threshold)  )
        {
            ret_val = true;
        }
    }
    // If moving forward take into account only front sensors
    // Uncomment next if clause only if back sensors are being mounted
    // else if (m_speed < 0)
    //         {
    //             threshold = m_speed*8+(1/m_speed)*10;
    //             if ( (m_senLeftBach.getDistance() <= threshold*(-1)) || (m_senRightBach.getDistance() <= threshold*(-1))  )
    //             {
    //                 ret_val = true;
    //             }
    //         }

    // Return value indicating whether brake should be activated
    return ret_val;
}

/** @brief  Serial callback method
  *
  * Serial callback attaching serial callback to CSafetyStopFunction object
  *
  * @param  obj                 CSafetyStopFunction controller object
  * @param  a                   string to read data from
  * @param b                   string to write data to
  * 
  */
void CSafetyStopFunction::staticSerialCallback(void* obj,char const * a, char * b)
{
    CSafetyStopFunction* self = static_cast<CSafetyStopFunction*>(obj);
    self->serialCallback(a,b);
}

/** @brief  Serial callback actions
  *
  * Serial callback method setting controller to values received
  *
  * @param  a                   string to read data from
  * @param b                   string to write data to
  * 
  */
void CSafetyStopFunction::serialCallback(char const * a, char * b){
        int l_isActivate=0;
        uint32_t l_res= sscanf(a,"%d",&l_isActivate);
        if(l_res==1){
            m_active=(l_isActivate>=1);
            sprintf(b,"ack;;");
        }else{
            sprintf(b,"sintax error;;");
        }
    }



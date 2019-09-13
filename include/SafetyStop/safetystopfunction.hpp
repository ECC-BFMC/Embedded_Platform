/**
  ******************************************************************************
  * @file    SafetyStopFunction.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-2017
  * @brief   This file contains the class declaration for the safety stop activation
  *          methods.
  ******************************************************************************
 */

/* Include guard */
#ifndef SAFETY_STOP_FUNCTION_HPP
#define SAFETY_STOP_FUNCTION_HPP

/* Required for the use of C_IR_DistanceSensorDriver */
#include <SHARP_IR_distance\IR_DistanceSensorDriver\ir_distancesensordriver.hpp>


/**
 * @brief It is used for deciding whether to activate safetyu stop depending on
 * distance sensors reading.
 * 
 */
class CSafetyStopFunction{
public:
    /* Construnctor */
    CSafetyStopFunction(C_IR_DistanceSensorDriver& f_senLeftFront
                        , C_IR_DistanceSensorDriver& f_senMidLeftFront
                        , C_IR_DistanceSensorDriver& f_senCenterFront
                        , C_IR_DistanceSensorDriver& f_senMidRightFront
                        , C_IR_DistanceSensorDriver& f_senRightFront
                        // , C_IR_DistanceSensorDriver& f_senLeftBach
                        // , C_IR_DistanceSensorDriver& f_senRightBach
                        );

    /* Destructor */
    ~CSafetyStopFunction();

    /* Function for activating safety stop */
    bool isSafetyStopActive(float f_speed, float f_angle);
    /* Serial callback method */
    static void staticSerialCallback(void* obj,char const * a, char * b);
private:
    /* Serial callback actions */
    void serialCallback(char const * a, char * b);
    /* The functionality active state*/
    bool  m_active;
    /* Current car speed */
    float m_speed;
    /* Current steering angle */
    float m_angle;
    /* Front sensors */
    C_IR_DistanceSensorDriver& m_senLeftFront;
    C_IR_DistanceSensorDriver& m_senMidLeftFront;
    C_IR_DistanceSensorDriver& m_senCenterFront;
    C_IR_DistanceSensorDriver& m_senMidRightFront;
    C_IR_DistanceSensorDriver& m_senRightFront;
    /* Back sensors */
    // C_IR_DistanceSensorDriver& m_senLeftBach;
    // C_IR_DistanceSensorDriver& m_senRightBach;
    /* Serial communication member */
    /* USED FOR DEBUG */
};

/* Include guard */
#endif








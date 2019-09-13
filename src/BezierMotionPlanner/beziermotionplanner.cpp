/**
  ******************************************************************************
  * @file    BezierMotionPlanner.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class definition for the Bezier Motion
  *          Planner methods.
  ******************************************************************************
 */

#include <iostream>
#include <BezierMotionPlanner/bezierMotionplanner.hpp>

namespace planner{
    
    /**
     * @brief Construct a new CBezierMotionPlanner::CBezierMotionPlanner object
     * 
     */
    CBezierMotionPlanner::CBezierMotionPlanner()
    {
        this->isInitialized=false;
    }

    /**
     * @brief Construct a new CBezierMotionPlanner::CBezierMotionPlanner object
     * 
     * @param isForward             Forward movement flag
     * @param a                     Point A
     * @param b                     Point B
     * @param c                     Point C
     * @param d                     Point D
     * @param motion_duration_i     The motion duration in second. 
     * @param timestep_i            The base period of the planner. (Sample time) 
     */
    CBezierMotionPlanner::CBezierMotionPlanner(bool                    isForward,
                                            std::complex<float>     a,
                                            std::complex<float>     b,
                                            std::complex<float>     c,
                                            std::complex<float>     d,
                                            float                   motion_duration_i,
                                            float                   timestep_i)
        : isForward(isForward)
        , bezierCurve(a,b,c,d)
        , motion_duration(motion_duration_i)
        , time_step(timestep_i)
    {
        this->bezierValueInput_step=1.0/(int)(this->motion_duration/this->time_step);
        this->next_bezierValueInput=0.0;
        this->isInitialized=true;
    }


    /**
     * @brief Set motion planner parameters
     * 
     * @param isForward             Forward movement flag
     * @param a                     Point A
     * @param b                     Point B
     * @param c                     Point C
     * @param d                     Point D
     * @param motion_duration_i     The motion duration in second. 
     * @param timestep_i            The base period of the planner. (Sample time) 
     */
    void CBezierMotionPlanner::setMotionPlannerParameters(bool                    isForward,
                                                        std::complex<float>     a,
                                                        std::complex<float>     b,
                                                        std::complex<float>     c,
                                                        std::complex<float>     d,
                                                        float                   motion_duration_i,
                                                        float                   timestep_i)
    {
        this->isForward=isForward;
        this->motion_duration=motion_duration_i;
        this->time_step=timestep_i;
        this->bezierCurve.setBezierCurve(a,b,c,d);
        this->bezierValueInput_step=1.0/(int)(this->motion_duration/this->time_step);
        this->next_bezierValueInput=0.0;
        this->isInitialized=true;                        
    }

    /**
     * @brief Destroy the CBezierMotionPlanner::CBezierMotionPlanner object
     * 
     */
    CBezierMotionPlanner::~CBezierMotionPlanner()
    {
    }


    /**
     * @brief Get the Bezier curve.
     * 
     */
    math::BezierCurve<float> CBezierMotionPlanner::getBezierCurve()
    {
        return this->bezierCurve;
    }

    /**
     * @brief Get the next control parameters. It calculates the velocity and angle, and increase input value. 
     * 
     * @return It returns the next forward velocity and direction angle. 
     */
    std::pair<float,float> CBezierMotionPlanner::getNextVelocity(){
        std::pair<float,float> commands=this->getVelocity(next_bezierValueInput);
        this->next_bezierValueInput+=this->bezierValueInput_step;
        return commands;
    }

    /**
     * @brief Get the forward velocity and steering angle, base on the given input value.
     * 
     * @param input_value The input value have to belong to interval [0,1].
     * @return It returns a pair of number, where the fist variable and the second contains the forward velocity and the steering angular, respectively.
     */
    std::pair<float,float> CBezierMotionPlanner::getVelocity(float input_value)
    {
        if(!this->isInitialized) return std::pair<float,float>(0,0);
        
        std::complex<float> dS=this->bezierCurve.get_FO_DerivateValue(input_value);

        float dl_absolute=sqrt((dS*std::conj(dS)).real());//[0,1]//Length of the vector
        float dl_real=dl_absolute/this->motion_duration;

        std::complex<float> ddS=this->bezierCurve.get_SO_DerivateValue(input_value);

        std::complex<float> correctorValue(0,-2);
        std::complex<float> temp1=(dS*std::conj(ddS)-std::conj(dS)*ddS)/correctorValue;
        float num=temp1.real();
        if(dl_absolute==0)
        {
            return std::pair<float,float>(dl_real,0);
        }

        float k=num/pow(dl_absolute,3);

        float angle_rad=atan(k*WHEELBASE);
        float angle_deg=(180.f/M_PI)*angle_rad;

        std::pair<float,float> commands(dl_real,angle_deg);
        return commands;
    }

    /**
     * @brief Get the state of the planner.
     * 
     * @return true The planner has valid value. 
     * @return false The planner finished the last given curve, it cannnot get correct parameters. 
     */
    bool CBezierMotionPlanner::hasValidValue()
    {
        return (next_bezierValueInput>=0 && next_bezierValueInput<=1);
    }

    /** @brief  Get the direction of the motion
     *
     *  
     * @return true The motion direction is forward. 
     * @return false The motion direction is backward. 
     */
    bool CBezierMotionPlanner::getForward()
    {
        return this->isForward;
    }

}; // namespace planner
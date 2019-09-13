/**
  ******************************************************************************
  * @file    BezierMotionPlanner.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class declaration for the Bezier Motion
  *          Planner methods.
  ******************************************************************************
 */

/* include guard */
#ifndef MOTIONPLANNER_HPP
#define MOTIONPLANNER_HPP

#include <complex>
#include <utility>
#include <math.h>
#include <BezierCurve/BezierCurve.hpp>

/* Module defines */
#define WHEELBASE 0.265

namespace planner{

       /** CBezierMotionPlanner class. 
        * @brief This class implements the functionality of planner to keep the robot on the given Bezier curve.
        * 
        */
        class CBezierMotionPlanner
        {
        public:
                /* Constructor */
                CBezierMotionPlanner();
                /* Constructor */
                CBezierMotionPlanner(bool              isForward,
                                std::complex<float>     a,
                                std::complex<float>     b,
                                std::complex<float>     c,
                                std::complex<float>     d,
                                float                   motion_duration_i,
                                float                   timestep_i);
                /* Set parameters */
                void setMotionPlannerParameters(bool                    isForward,
                                                std::complex<float>     a,
                                                std::complex<float>     b,
                                                std::complex<float>     c,
                                                std::complex<float>     d,
                                                float                   motion_duration_i,
                                                float                   timestep_i);
                /* Destructor */
                virtual ~CBezierMotionPlanner();
                /* Get Bezier Curve */
                math::BezierCurve<float> getBezierCurve();
        //      Velocity longitudinal and angular
                /* Get next velocity value */
                std::pair<float,float> getNextVelocity();
        //      Velocity longitudinal and angular, Input value [0,1]
                /* Get velocity value */
                std::pair<float,float> getVelocity(float input_value);
                /* Next value valid */
                bool hasValidValue();
                /* Is movement forward */
                bool getForward();

        private:
                /* forward movement indicator */
                bool isForward;
                /* Bezier Curve object */
                math::BezierCurve<float> bezierCurve;
                /* motion duration */
                float motion_duration;
                /* time step */
                float time_step;
                /* Bezier value input step */
                float bezierValueInput_step;
                /* next Bezier value */
                float next_bezierValueInput;
                /* intialization indicator */
                bool isInitialized;
        };

}; // namespace planner

#endif // CMotionPlanner_HPP

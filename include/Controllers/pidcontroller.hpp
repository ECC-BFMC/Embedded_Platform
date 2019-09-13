#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <cstdio>
#include <Linalg/linalg.h>
#include <SystemModels/systemmodels.hpp>
#include <mbed.h>

namespace controllers
{
    namespace siso
    {
        
        /**
         * @brief General interface class for the controller
         * 
         * @tparam T type of the variables (float, double)
         */
        template<class T>
        class IController{
            public:
                virtual T calculateControl(const T&)=0;
                virtual void clear()=0;
        };

        /**
         * @brief It generates a discrete transferfunction for realizing a proportional–integral–derivative controller, which is discretized by the Euler’s method.
         * 
         * @tparam T type of the variables (float, double) 
         */
        template<class T>
        class CPidController:public IController<T>
        {
            public:
                /* Discrete transfer function type */
                using CPidSystemmodelType = systemmodels::lti::siso::CDiscreteTransferFucntion<T,3,3>;
                
                /* Constructor */
                CPidController(T              f_kp
                              ,T              f_ki
                              ,T              f_kd
                              ,T              f_tf
                              ,T              f_dt);
                CPidController(CPidSystemmodelType f_pid,T f_dt);

                /* Overloaded operator */
                T calculateControl(const T& f_input);
                /* Serial callback */
                static void staticSerialCallback(void* obj,char const * a, char * b);
                /* Initialization */
                void clear();
            private:
                #ifdef CONTROL_TEST_HPP
                    FRIEND_TEST(PidControlTest,Initialization);
                    FRIEND_TEST(PidControlTest,Setter);
                #endif 
                /* Controller setting */
                void setController(T              f_kp
                                  ,T              f_ki
                                  ,T              f_kd
                                  ,T              f_tf);
                /* Serial callback implementation */
                void serialCallback(char const * a, char * b);

                

                /* Discrete transfer function */
                CPidSystemmodelType     m_pidTf;


                /* delta-time term */
                T                 m_dt;

        };
        /* Include function definitions */
        #include "PidController.inl"
    }; // namespace siso
}; // namespace controllers   



#endif
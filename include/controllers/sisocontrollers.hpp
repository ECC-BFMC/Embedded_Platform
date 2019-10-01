#ifndef SISO_CONTROLLERS_H
#define SISO_CONTROLLERS_H

#include <cstdio>
#include <linalg/linalg.h>
#include <systemmodels/systemmodels.hpp>
#include <mbed.h>

namespace controllers
{
    namespace siso
    {
        
        /**
         * @brief General interface class for the controller with single input and single output
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
                /* Discrete transferfunction type */
                using CPidSystemmodelType = systemmodels::lti::siso::CDiscreteTransferFunction<T,3,3>;
                
                /* Constructor */
                CPidController(T              f_kp
                              ,T              f_ki
                              ,T              f_kd
                              ,T              f_tf
                              ,T              f_dt);
                CPidController(CPidSystemmodelType f_pid,T f_dt);

                /* Calculate the control signal based the input error. */
                T calculateControl(const T& f_input);
                /* Serial callback */
                static void staticSerialCallback(void* obj,char const * a, char * b);
                /* Serial callback implementation */
                void serialCallback(char const * a, char * b);

                /* Set to zero the previous values in the transferfunction  */
                void clear();
            private:
                /* Set the controller's parameters. */
                void setController(T              f_kp
                                  ,T              f_ki
                                  ,T              f_kd
                                  ,T              f_tf);

                /* Discrete transferfunction */
                CPidSystemmodelType     m_pidTf;


                /* Sampling time */
                T                 m_dt;

        };
        /* Include function definitions */
        #include "sisocontrollers.tpp"
    }; // namespace siso
}; // namespace controllers   



#endif
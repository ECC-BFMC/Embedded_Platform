/**
  ******************************************************************************
  * @file    SystemModels/systemmodels.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-2017
  * @brief   This file contains the class declarations for system model.
  ******************************************************************************
 */

/* Inclusion guard */
#ifndef SYSTEM_MODELS_HPP
#define SYSTEM_MODELS_HPP

#include <Linalg/linalg.h>

// Discrete System Models
namespace systemmodels{
    //Linear time variant models
    namespace lti{
        namespace siso{
           /**
            * @brief Discrete transfer function
            * 
            * @tparam T The type of the variable 
            * @tparam NNum The order of the polynomial
            * @tparam NDen The order of the polynomial
            */
            template <class T,uint32_t NNum,uint32_t NDen>
            class CDiscreteTransferFucntion{
                public:
                    using CDenType          =   linalg::CMatrix<T,NDen,1>;
                    using CDenModType       =   linalg::CMatrix<T,NDen-1,1>;
                    using CNumType          =   linalg::CMatrix<T,NNum,1>;
                    // using CNumModType       =   linalg::CMatrix<T,NNum-1,1>;
                    using CInputMem         =   linalg::CMatrix<T,1,NNum>;
                    using COutputMem        =   linalg::CMatrix<T,1,NDen-1>;
                    /* Constructor */
                    CDiscreteTransferFucntion();

                    CDiscreteTransferFucntion(const CNumType& f_num,const CDenType& f_den);

                    /* Clear memory */
                    void clearMemmory();
                    /* Shift memory */
                    template<uint32_t N>
                    void shiftMemory(linalg::CMatrix<T,1,N>& f_mem);
                    /* Operator */
                    T operator()(const T& f_input);
                    /* Set num */
                    void setNum(const CNumType& f_num);
                    /* Set den */
                    void setDen(const CDenType& f_den);

                    const CNumType& getNum();
                    const CDenModType& getDen();
                    float getDenCurrent();
                    /* Get output */
                    T getOutput();

                private:
                
                    #ifdef SYSTEMMODEL_TEST_HPP
                        FRIEND_TEST(CDiscreteTransferFucntionTest,Reset);
                    #endif

                    /* nominator type */
                    CNumType    m_num;
                    // CDenType       m_den;
                    /* denominator type */
                    CDenModType    m_den;
                    T              m_denCoef;
                    /* input memory */
                    CInputMem      m_memInput;
                    /* output memory */
                    COutputMem     m_memOutput;
            };
        }

    };
    // Nonlinea time invariant models
    namespace nlti{
        //Multi-input and multi-output 
        namespace mimo{
            // Discrete time system model of a system nonlinear time invariant with multi-input and multi-output
            //  T       -variable type
            //  NA      -number of inputs
            //  NB      -number of states
            //  NC      -number of outputs
            template <class T,uint32_t NA, uint32_t NB,uint32_t NC>
            class CDiscreteTimeSystemModel{
                public:
                    using CStatesType           =   linalg::CMatrix<T,NB,1>;
                    using CInputType            =   linalg::CMatrix<T,NA,1>;
                    using COutputType           =   linalg::CMatrix<T,NC,1>;
                    /* Constructor */
                    CDiscreteTimeSystemModel(const double           f_dt);//:m_states(),m_dt(f_dt){}
                    /* Constructor */
                    CDiscreteTimeSystemModel(const CStatesType&     f_states
                                            ,const double           f_dt);
                    //State transition model
                    //Calculate the system state depending on input, after calculation the relevant class members need to be syncronized.
                    virtual CStatesType update(const CInputType&       f_input)=0;
                    //State observation model
                    //Calculate the system output depending on input, after calculation the relevant class members need to be syncronized.
                    virtual COutputType calculateOutput(const CInputType&  f_input)=0;
                    // GETTERS
                    // The method returns the current system states
                    CStatesType         getStates();//{return m_states;}
                    // The method returns the current system output
                    COutputType         getOutput(){return m_outputs;}
                    float               getTimeStep(){return m_dt;}
                    // SETTERS
                    void                setStates(const CStatesType& f_states){
                        m_states=f_states;
                    }
                protected:
                    // States
                    CStatesType                         m_states;
                    // Output
                    COutputType                         m_outputs;
                    // Time step
                    const double                        m_dt;
                private:        
            };

            // the state transition and observation matrices defined by Jacobians
            //  T       -variable type
            //  NA      -number of inputs
            //  NB      -number of states
            //  NC      -number of outputs
            template <class T,uint32_t NA, uint32_t NB,uint32_t NC>
            class CJacobianMatrices{
                public:
                    using CStatesType           =   linalg::CMatrix<T,NB,1>;
                    using CInputType            =   linalg::CMatrix<T,NA,1>;
                    // using COutputType           =   linalg::CMatrix<T,NC,1>;
                    using CJMTransitionType     =   linalg::CMatrix<T,NB,NB>;
                    using CJMObservationType    =   linalg::CMatrix<T,NC,NB>;

                    //The method calculates and returns  the states transition matrix 
                    virtual CJMTransitionType getJMTransition(const CStatesType&     f_states
                                                             ,const CInputType&      f_inputs)=0;
                    // The method calculates and returns  the states observation matrix
                    virtual CJMObservationType getJMObservation(const CStatesType&      f_states
                                                               ,const CInputType&       f_input)=0;
                private:
            };
        };
    };
};

#include "systemmodels.inl"

#endif
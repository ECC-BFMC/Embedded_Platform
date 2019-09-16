
/**
  ******************************************************************************
  * @file    AckermannModel.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class declaration for the Ackermann model.
  ******************************************************************************
 */

/* Include guard */
#ifndef ACKERMANN_MODEL_HPP
#define ACKERMANN_MODEL_HPP

#include <math.h>
#include <SystemModels/systemmodels.hpp>
#include <Examples/SystemModels/ackermanntypes.hpp>

/* Definition of PI value */
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

/* Definition of degrees to radians transformation */
#ifndef DEG2RAD 
    #define DEG2RAD M_PI/180.0
#endif

namespace examples
{
    namespace systemmodels
    {
        namespace ackermannmodel
        {
            /* System model type */
            template<class T,uint32_t NA,uint32_t NB,uint32_t NC>
            using CSystemModelType      = ::systemmodels::nlti::mimo::CDiscreteTimeSystemModel<T,NA,NB,NC>;
            /* Jacobian matrix*/
            template<class T,uint32_t NA,uint32_t NB,uint32_t NC>
            using CJacobianMatricesType = ::systemmodels::nlti::mimo::CJacobianMatrices<T,NA,NB,NC>;
            //!  CAckermannModel class. 
            /*!
            * It inherits class CSystemModelType. 
            */
            class CAckermannModel:public CSystemModelType<double,2,10,5>
            {
                public:
                    /* Constructor */
                    CAckermannModel(const double       f_dt     
                                    ,const double      f_gamma
                                    ,const double      f_wb
                                    ,const double      f_b
                                    ,const double      f_J
                                    ,const double      f_K
                                    ,const double      f_R
                                    ,const double      f_L);
                    /* Constructor */
                    CAckermannModel(const CStatesType&      f_states
                                    ,const double            f_dt     
                                    ,const double            f_gamma
                                    ,const double            f_wb
                                    ,const double            f_b
                                    ,const double            f_J
                                    ,const double            f_K
                                    ,const double            f_R
                                    ,const double            f_L);
                    /* Update method */
                    CStatesType update(const CInputType&       f_input);
                    
                    /* Calculate output method */
                    COutputType calculateOutput(const CInputType&  f_input);
                private:
                    /* gamma=Meter/Rotation */
                    const double m_gamma;
                    /* Wheel base distance in meter */
                    const double m_wb;
                    /* Motor Constants */
                    const double m_bJ,m_KJ,m_KL,m_RL,m_L;      
            };

            //!  CJMAckermannModel class. 
            /*!
            * It inherits class CJacobianMatricesType. 
            */
            class CJMAckermannModel:public CJacobianMatricesType<double,2,10,5>
            {
                public:
                    CJMAckermannModel   (double          f_dt
                                        ,double          f_gamma
                                        ,double          f_wb
                                        ,double          f_b
                                        ,double          f_J
                                        ,double          f_K
                                        ,double          f_R
                                        ,double          f_L)
                                        :m_dt(f_dt)
                                        ,m_gamma(f_gamma)
                                        ,m_wb(f_wb)
                                        ,m_bJ(f_b/f_J)
                                        ,m_KJ(f_K/f_J)
                                        ,m_KL(f_K/f_L)
                                        ,m_RL(f_R/f_L)
                    {
                        m_ObservationMatrix=initObservationMatrix();
                    }
                


                    CJMObservationType getJMObservation(    const CStatesType&       f_states
                                                            ,const CInputType&       f_input){
                        return m_ObservationMatrix;
                    }
                
                private:
                    linalg::CMatrix<double,5,10> initObservationMatrix(){
                        linalg::CMatrix<double,5,10> l_data;
                        l_data[0][2]=l_data[1][3]=1.f/m_dt;
                        l_data[0][4]=l_data[1][5]=-1.f/m_dt;
                        l_data[2][7]=1.f;
                        l_data[3][8]=m_gamma;
                        l_data[4][9]=1.f;
                        return l_data;
                    }

                    inline void  setJacobianStateMatrixWithOne(CJMTransitionType& f_matrix){
                        f_matrix[0][0]=f_matrix[1][1]=1;
                        f_matrix[4][2]=f_matrix[5][3]=1;
                        f_matrix[6][6]=1;
                        return;
                    }

                public:
                    CJMTransitionType getJMTransition(       const CStatesType&       f_states
                                                            ,const CInputType&       f_input){

                        CJMTransitionType l_data(CJMTransitionType::zeros());
                        setJacobianStateMatrixWithOne(l_data);
                        CState l_states(f_states);
                        CInput l_input(f_input);

                        //Setting values in the jacobian matrix
                        l_data[0][2]=m_dt;
                        l_data[1][3]=m_dt;
                        
                        l_data[2][6]=-m_gamma*l_states.omega()*sin(l_states.teta_rad());
                        l_data[2][8]=m_gamma*cos(l_states.teta_rad());

                        l_data[3][6]=m_gamma*l_states.omega()*cos(l_states.teta_rad());
                        l_data[3][8]=m_gamma*sin(l_states.teta_rad());

                        double l_alpha_rad=l_input.alpha()*DEG2RAD;
                        l_data[7][8]=l_data[6][8]=m_dt*m_gamma*tan(l_alpha_rad)/m_wb;
                        //l_data[7][11]=l_data[6][11]=m_dt*m_gamma*f_state.omega/(m_wb*pow(cos(l_alpha_rad),2));

                        l_data[8][8]=(1-m_dt*m_bJ);
                        l_data[8][9]=m_dt*m_KJ;
                        l_data[9][8]=m_dt*m_KL;
                        l_data[9][9]=(1-m_dt*m_RL);
                        //l_data[9][10]=m_dt;
                        return l_data;
                    }

                private:
                    /* Constant values  */
                    /* Time step  */
                    const double m_dt;
                    /* gamma=Meter/Rotation  */
                    const double m_gamma;
                    /* Wheel base distance in meter  */
                    const double m_wb;
                    /* Motor Constants  */
                    const double m_bJ,m_KJ,m_KL,m_RL;
                    /* Please leave this matrix to be the last member in this class, 
                    as it will be initialized  */
                    linalg::CMatrix<double,5,10> m_ObservationMatrix;    
            };
        };
    };
};

#endif
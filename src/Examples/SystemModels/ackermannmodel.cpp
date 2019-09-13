/**
  ******************************************************************************
  * @file    AckermannModel.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class implementation for the Ackermann model.
  ******************************************************************************
 */

#include <Examples/SystemModels/ackermannmodel.hpp>

/* Specify model type */
using ackermannmodeltype = systemmodels::nlti::mimo::CDiscreteTimeSystemModel<double,2,10,5>;

/** \brief  Constructor for the CAckermannModel class
 *
 *  Constructor method
 *
 *  \param[in] f_states       reference to initial system states 
 *  \param[in] f_dt           sample time
 *  \param[in] f_gamma        reduction factor from motor to wheel
 *  \param[in] f_wb           wheel base
 *  \param[in] f_b            motor viscous friction constant
 *  \param[in] f_J            moment of inertia of the rotor
 *  \param[in] f_K            motor torque constant
 *  \param[in] f_R            electric resistance 
 *  \param[in] f_L            electric inductance
 * 
 */
examples::systemmodels::ackermannmodel::CAckermannModel::CAckermannModel(
                                     const CStatesType&     f_states
                                    ,const double            f_dt     
                                    ,const double            f_gamma
                                    ,const double            f_wb
                                    ,const double            f_b
                                    ,const double            f_J
                                    ,const double            f_K
                                    ,const double            f_R
                                    ,const double            f_L)
    :CSystemModelType<double,2,10,5>(f_states,f_dt)
    ,m_gamma(f_gamma)
    ,m_wb(f_wb)
    ,m_bJ(f_b/f_J)
    ,m_KJ(f_K/f_J)
    ,m_KL(f_K/f_L)
    ,m_RL(f_R/f_L)
    ,m_L(f_L)
{
}

/** \brief  Constructor for the CAckermannModel class
 *
 *  Constructor method
 *
 *  \param[in] f_dt           sample time
 *  \param[in] f_gamma        reduction factor from motor to wheel
 *  \param[in] f_wb           wheel base
 *  \param[in] f_b            motor viscous friction constant
 *  \param[in] f_J            moment of inertia of the rotor
 *  \param[in] f_K            motor torque constant
 *  \param[in] f_R            electric resistance 
 *  \param[in] f_L            electric inductance
 * 
 */
examples::systemmodels::ackermannmodel::CAckermannModel::CAckermannModel(
                                     const double      f_dt     
                                    ,const double      f_gamma
                                    ,const double      f_wb
                                    ,const double      f_b
                                    ,const double      f_J
                                    ,const double      f_K
                                    ,const double      f_R
                                    ,const double      f_L)
    :CSystemModelType<double,2,10,5>(f_dt)
    ,m_gamma(f_gamma)
    ,m_wb(f_wb)
    ,m_bJ(f_b/f_J)
    ,m_KJ(f_K/f_J)
    ,m_KL(f_K/f_L)
    ,m_RL(f_R/f_L)
    ,m_L(f_L)
{
}

/** \brief  Update method
 *
 *  Method for updating system states.
 *
 *  \param[in] f_input        reference to input type object
 *  \return    system states
 */
ackermannmodeltype::CStatesType 
    examples::systemmodels::ackermannmodel::CAckermannModel::update(
      const CInputType&       f_input)
{
    CState l_states=m_states;
    CInput l_input=f_input;
    l_states.x()+=m_dt*l_states.x_dot();
    l_states.y()+=m_dt*l_states.y_dot();

    l_states.x_dot_prev()=l_states.x_dot();
    l_states.y_dot_prev()=l_states.y_dot();

    l_states.x_dot()=m_gamma*l_states.omega()*cos(l_states.teta_rad());
    l_states.y_dot()=m_gamma*l_states.omega()*sin(l_states.teta_rad());

    double l_alpha_rad=l_input.alpha()*DEG2RAD;
    l_states.teta_rad_dot()=m_gamma*l_states.omega()*tan(l_alpha_rad)/m_wb;
    l_states.teta_rad()+=m_dt*l_states.teta_rad_dot();

    double omega_k_1=(1-m_dt*m_bJ)*l_states.omega()+m_dt*m_KJ*l_states.i();//next state of the motor's rotation speed
    l_states.i()=-1*m_dt*m_KL*l_states.omega()+(1-m_dt*m_RL)*l_states.i()+m_dt*l_input.v()/m_L;
    l_states.omega()=omega_k_1;
    m_states=l_states;
    return l_states;
}

/** \brief  Calculate output method
 *
 *  Method for calculating output depending on input.
 *
 *  \param[in] f_input        reference to input type object
 *  \return    systemoutputs
 */
ackermannmodeltype::COutputType 
    examples::systemmodels::ackermannmodel::CAckermannModel::calculateOutput(
      const CInputType&  f_input)
{
    CState l_states=m_states;
    // CInput l_input=f_input;
    COutput l_outputs(COutputType::zeros());
    l_outputs.x_ddot()=l_states.x_dot()-l_states.x_dot_prev();
    l_outputs.y_ddot()=l_states.y_dot()-l_states.y_dot_prev();
    l_outputs.teta_rad_dot()=l_states.teta_rad_dot();
    l_outputs.speed()=l_states.omega()*m_gamma;
    // output.alpha()=f_input.alpha();
    l_outputs.i()=l_states.i();
    m_outputs=l_outputs;
    return l_outputs;
}
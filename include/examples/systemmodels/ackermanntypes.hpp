/**
  ******************************************************************************
  * @file    AckermannTypes.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class declaration and implementation for the
             Ackermann types.
  ******************************************************************************
 */

/* Include guard */
#ifndef ACKERMANN_TYPES_HPP
#define ACKERMANN_TYPES_HPP

#include <utils/linalg/linalg.h>

namespace examples
{
    namespace systemmodels
    {
        namespace ackermannmodel
        {
            class CState: public utils::linalg::CMatrix<double,10,1>
            {
                public:
                    CState():utils::linalg::CMatrix<double,10,1>(){}
                    CState(const utils::linalg::CMatrix<double,10,1>& f_matrix):utils::linalg::CMatrix<double,10,1>(f_matrix){}
                    double& x(){return m_data[0][0];}
                    const double& x() const {return m_data[0][0];}
                    
                    double& y(){return m_data[1][0];}
                    const double& y() const {return m_data[1][0];}
                    
                    double& x_dot(){return m_data[2][0];}
                    const double& x_dot() const {return m_data[2][0];}

                    double& y_dot(){return m_data[3][0];}
                    const double& y_dot() const{return m_data[3][0];}

                    double& x_dot_prev(){return m_data[4][0];}
                    const double& x_dot_prev() const{return m_data[4][0];}

                    double& y_dot_prev(){return m_data[5][0];}
                    const double& y_dot_prev() const{return m_data[5][0];}

                    double& teta_rad(){return m_data[6][0];}
                    const double& teta_rad() const{return m_data[6][0];}

                    double& teta_rad_dot(){return m_data[7][0];}
                    const double& teta_rad_dot() const{return m_data[7][0];}
                    
                    double& omega(){ return m_data[8][0];}
                    const double& omega() const{return m_data[8][0];}

                    double& i(){return m_data[9][0];}
                    const double& i() const{return m_data[9][0];}

                    // CState& operator=(const utils::linalg::CMatrix<double,10,1>& f_matrix){
                    //     for (uint32_t l_row = 0; l_row < 10; ++l_row)
                    //     {
                    //         for (uint32_t l_col = 0; l_col < 1; ++l_col)
                    //         {
                    //             this->m_data[l_row][l_col] = f_matrix[l_row][l_col];
                    //         }
                    //     }
                    //     return *this;
                    // }
            };

            class CControl:public utils::linalg::CMatrix<double,2,1>
            {
                public:
                    CControl():utils::linalg::CMatrix<double,2,1>(){}
                    CControl(const utils::linalg::CMatrix<double,2,1>& f_matrix):utils::linalg::CMatrix<double,2,1>(f_matrix){} 
                    double& v(){return m_data[0][0];}
                    const double& v()const{return m_data[0][0];}
                    double& alpha(){return m_data[1][0];}
                    const double& alpha()const{return m_data[1][0];}
            };

            class CObservation:public utils::linalg::CMatrix<double,5,1>
            {
                public:
                    CObservation():utils::linalg::CMatrix<double,5,1>(){}
                    CObservation(const utils::linalg::CMatrix<double,5,1>& f_matrix):utils::linalg::CMatrix<double,5,1>(f_matrix){}
                    double& x_ddot(){return m_data[0][0];}
                    const double& x_ddot() const{return m_data[0][0];}
                    double& y_ddot(){return m_data[1][0];}
                    const double& y_ddot()const {return m_data[1][0];}
                    double& teta_rad_dot(){return m_data[2][0];}
                    const double& teta_rad_dot()const{return m_data[2][0];}
                    double& speed(){return m_data[3][0];}
                    const double& speed()const {return m_data[3][0];}
                    
                    // double& alpha(){
                    //     return m_data[4][0];
                    // }
                    // const double& alpha()const{
                    //     return m_data[4][0];
                    // }
                    double& i(){return m_data[4][0];}
                    const double& i()const{return m_data[4][0];}
            };
        }; // namespace ackermannmodel
    }; // namespace systemmodels
}; // namespace examples


#endif
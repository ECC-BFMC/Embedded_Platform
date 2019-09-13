/**
  ******************************************************************************
  * @file    PolynomialFunction.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class declaration for the Plynomial Function
  *          methods.
  ******************************************************************************
 */

/* include guard */
#ifndef POLYNOMIALFUNCTION_HPP
#define POLYNOMIALFUNCTION_HPP

#include <stdint.h>
#include <math.h>
#include <iostream>


namespace math{
   /**
    * @brief It represents a one dimensional polynomial function
    * 
    * @tparam T The type of the Polynomial function
    * @tparam N The degree of the polynom. 
    */
    template <class T,int32_t N>
    class PolynomialFunction
    {
        public:
            /* Constructors  */
            PolynomialFunction();
            PolynomialFunction(T coefficients[N+1]);
            /* Destructors  */
            virtual ~PolynomialFunction();

    //        template<int32_t N2> void add(PolynomialFunction<T,N2> poli);
            /* Add */
            template<int32_t N2> math::PolynomialFunction<T,(N2<N?N:N2)> add(PolynomialFunction<T,N2> b);
            /* Multiply */
            template<int32_t N2> math::PolynomialFunction<T,(N2+N)> multip(PolynomialFunction<T,N2> b);
            /* Calculate value */
            T calculateValue(T input_value);
            /* First order derivate */
            math::PolynomialFunction<T,N-1> derivateFO();
            /* Get degree */
            int32_t getDegree();
            /* Get coefficient value */
            T getCoefficientValue(int32_t index);
            /* Set coefficient value */
            void setCoefficientValue(int32_t index,T value );
        private:
            /* Coefficients array */
            /**
             * @brief The list of the coeffients.
             * 
             */
            T coefficients[N+1];
    };

}; // namepace math

#include "polynomialfunction.inl"

#endif // POLYNOMIALFUNCTION_HPP

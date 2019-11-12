/**
Copyright 2019 Bosch Engineering Center Cluj and BFMC organizers 

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.



******************************************************************************
* @file    converters.hpp
* @author  RBRO/PJ-IU
* @version V1.0.0
* @date    2018-10-29
* @brief   
******************************************************************************
*/
#ifndef CONVERTERS_H
#define CONVERTERS_H

#include<cmath>
#include<stdint.h>
#include<array>

namespace signal{
  namespace controllers{
      /**
       * @brief Converter interface with single input and single output
       * 
       */
      class IConverter{
        public:
          virtual float operator()(float) = 0;
      };

      /**
       * @brief Polynomial converter.
       * 
       * @tparam NOrd The degree of the polynomial converter
       */
      template<uint8_t NOrd>
      class CConverterPolynom:public IConverter
      {
        public:
          CConverterPolynom(std::array<float,NOrd+1>);
          float operator()(float);

        private:
          std::array<float,NOrd+1> m_coeff;
      };

      /**
       * @brief A converter based on the set of break point and the multiple polynomial function.
       * 
       * @tparam NrBreak Number of the break.
       * @tparam NOrd    Degree of the polynomial converter. 
       */
      template<uint8_t NrBreak,uint8_t NOrd>
      class CConverterSpline:public IConverter
      {
        public:
          /** @brief Coefficient container types */
          using CCoeffContainerType = std::array<float,NOrd+1>;
          /** @brief Splines container type */
          using CSplineContainerType = std::array<CCoeffContainerType,NrBreak+1>;
          /** @brief Breaks container type */
          using CBreakContainerType = std::array<float,NrBreak>;

          CConverterSpline(CBreakContainerType f_breaks,CSplineContainerType f_splines);
          float operator()(float);
        private:
          float splineValue( CCoeffContainerType,float);

          CBreakContainerType     m_breaks;
          CSplineContainerType    m_splines;
          
      };

      #include "converters.tpp"
  }; //namespace controllers

}; // namespace signal


#endif // CONVERTERS_H

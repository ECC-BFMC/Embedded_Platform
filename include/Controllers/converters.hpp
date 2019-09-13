/**
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
        using CCoeffContainerType = std::array<float,NOrd+1>;
        using CSplineContainerType = std::array<CCoeffContainerType,NrBreak+1>;
        using CBreakContainerType = std::array<float,NrBreak>;


        CConverterSpline(CBreakContainerType f_breaks,CSplineContainerType f_splines);
        float operator()(float);
      private:
        float splineValue( CCoeffContainerType,float);

        CBreakContainerType     m_breaks;
        CSplineContainerType    m_splines;
        
    };

    #include "converters.inl"
}; //namespace controllers



#endif // CONVERTERS_H

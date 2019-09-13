
namespace math{

/**
 * @brief Construct a new Polynomial Function< T, N>:: Polynomial Function object
 * 
 */
template<class T,int32_t N>
PolynomialFunction<T,N>::PolynomialFunction()
{
    for(int i=0;i<=N;++i){
        this->coefficients[i]=static_cast<T>(0);
    }
}

/**
 * @brief Construct a new Polynomial Function< T, N>:: Polynomial Function object
 * 
 * @param coefficients The list of the coefficients for the polynomial function.
 */
template<class T,int32_t N>
PolynomialFunction<T,N>::PolynomialFunction(T coefficients[N+1])
{
    for(int i=0;i<=N;++i){
        this->coefficients[i]=coefficients[i];
    }
}

/**
 * @brief Destroy the Polynomial Function< T, N>:: Polynomial Function object
 * 
 */
template<class T,int32_t N>
PolynomialFunction<T,N>::~PolynomialFunction()
{
}

/**
 * @brief Get the degree of the polynomial function
 * 
 * @return The degree of the polynom
 */
template<class T,int32_t N>
int32_t PolynomialFunction<T,N>::getDegree()
{
    return N;
}

/**
 * @brief Get coefficient value
 * 
 * @param index         The index of the coefficient
 * @return The value of the coefficient
 */
template<class T,int32_t N>
T PolynomialFunction<T,N>::getCoefficientValue(int32_t index)
{
    if(index>N || index<0) return static_cast<T>(0);
    return this->coefficients[index];
}

/**
 * @brief Set the value of the coefficient
 * 
 * @param index     The index of the coefficient
 * @param value     The new value of the coefficient
 */
template<class T,int32_t N>
void PolynomialFunction<T,N>::setCoefficientValue(int32_t index, T value ){
    if(index<=N){
        this->coefficients[index]=value;
    }
}

/**
 * @brief It sums the polynom with the input polynom and return a result in the new polynom.
 * 
 * @param b         The input polynom.
 * @return The result polynom.
 */
template<class T,int32_t N>
template<int32_t N2>
math::PolynomialFunction<T,(N2<N?N:N2)> PolynomialFunction<T,N>::add(PolynomialFunction<T,N2> b){
    math::PolynomialFunction<T,N2<N?N:N2> p;
    for(int i=0;i<=N2 || i<=N;++i){
        p.setCoefficientValue(i,this->getCoefficientValue(i)+b.getCoefficientValue(i));
    }
    return p;
}


template<class T,int32_t N>
template<int32_t N2>
/**
 * @brief It multiply the polynom with the given polynom and return the result polynom.
 *  
 * @param b     The input polynom
 * @return The result polynom
 */
math::PolynomialFunction<T,(N2+N)> PolynomialFunction<T,N>::multip(PolynomialFunction<T,N2> b){
    math::PolynomialFunction<T,(N2+N)> result;
    for(int32_t i=0;i<=N2+N;++i){
        T sum=static_cast<T>(0);
        for(int32_t j=0;j<=i;++j){
            sum+=this->getCoefficientValue(j)*b.getCoefficientValue(i-j);
        }
        result.setCoefficientValue(i,sum);
    }

    return result;
}

template<class T,int32_t N>
/**
 * @brief Calculate the polynom value 
 * 
 * @param input_value   The given value 
 * @return The result value of the polynom. 
 */
T PolynomialFunction<T,N>::calculateValue(T input_value){
    T output_value=static_cast<T>(0);
    for(int32_t i=0;i<=N;++i){
        output_value+=this->coefficients[i]*static_cast<T>(pow(input_value,i));
    }
//    std::cout<<"OOO"<<output_value<<std::endl;
    return output_value;
}

template<class T, int32_t N>
/**
 * @brief It calcutes the first derivate of the polinom and return it by creating a new polynomial function object.
 * 
 * @return The result polynom.
 */
math::PolynomialFunction<T,N-1> PolynomialFunction<T,N>::derivateFO(){
    math::PolynomialFunction<T,N-1> derivate;
    for(int32_t i=0;i<N;++i){
        T coeff=static_cast<T>(i+1);
        derivate.setCoefficientValue(i,coeff*this->getCoefficientValue(i+1));
    }
    return derivate;
};

};

namespace math{

    /**
     * @brief Construct a new Bezier Curve< T>:: Bezier Curve object
     * 
     */
    template<class T>
    BezierCurve<T>::BezierCurve(){
    }

    /**
     * @brief Construct a new Bezier Curve< T>:: Bezier Curve object
     * 
     * @param a point A
     * @param b point B
     * @param c point C
     * @param d point D
     */
    template<class T>
    BezierCurve<T>::BezierCurve(std::complex<T> a,std::complex<T> b,std::complex<T> c,std::complex<T> d){
        this->points[0]=a;
        this->points[1]=b;
        this->points[2]=c;
        this->points[3]=d;

        this->bezierCurve=this->CP2PF();
        this->FOder_bezierCurve=this->bezierCurve.derivateFO();
        this->SOder_bezierCurve=this->FOder_bezierCurve.derivateFO();
    }

    /**
     * @brief Construct a new Bezier Curve< T>:: Bezier Curve object
     * 
     * @param points Array of points 
     */
    template<class T>
    BezierCurve<T>::BezierCurve(std::complex<T> points[BEZIER_ORDER+1])
    {
        for(int32_t i=0;i<=BEZIER_ORDER;++i){
            this->points[i]=points[i];
        }
        this->bezierCurve=this->CP2PF();
        this->FOder_bezierCurve=this->bezierCurve.derivateFO();
        this->SOder_bezierCurve=this->FOder_bezierCurve.derivateFO();
    }

    /**
     * @brief Destroy the Bezier Curve< T>:: Bezier Curve object
     * 
     */
    template<class T>
    BezierCurve<T>::~BezierCurve()
    {
        //dtor
    }

    /**
     * @brief This metohd create a new polynomial function based on the complex points.
     * 
     * @return The result polynomial function represent the Bezier curve.
     */
    template<class T>
    math::PolynomialFunction<std::complex<float>,BEZIER_ORDER> BezierCurve<T>::CP2PF(){
        math::PolynomialFunction<std::complex<float>,BEZIER_ORDER> pf;

        const std::complex<T> temp_cst_3(3,0);
        const std::complex<T> temp_cst_2(2,0);

        std::complex<T> coef1=temp_cst_3*(this->points[1]-this->points[0]);
        std::complex<T> coef2=temp_cst_3*(this->points[0]-temp_cst_2*this->points[1]+this->points[2]);
        std::complex<T> coef3=temp_cst_3*(this->points[1]-this->points[2])+this->points[3]-this->points[0];
        pf.setCoefficientValue(0,this->points[0]);
        pf.setCoefficientValue(1,coef1);
        pf.setCoefficientValue(2,coef2);
        pf.setCoefficientValue(3,coef3);
        return pf;
    }

    /**
     * @brief Get the point on the bezier curve. 
     * 
     * @param input_value  The input value, it must belong to interval [0,1]. 
     * @return The point as complex number
     */
    template<class T>
    std::complex<T> BezierCurve<T>::getValue(float input_value){
        T input_value_T=static_cast<T>(input_value);
        return this->bezierCurve.calculateValue(input_value);
    }

    template<class T>
    /**
     * @brief Get the value of the first order derivative of Bezier curve
     * 
     * @param input_value The input value [0,1]
     * @return The point as a complex number.
     * 
     */
    std::complex<T> BezierCurve<T>::get_FO_DerivateValue(float input_value){
        T input_value_T=static_cast<T>(input_value);
        return this->FOder_bezierCurve.calculateValue(input_value_T);
    }

    /**
     * @brief Get the value of the second order derivative of Bezier Curve.
     * 
     * @param input_value The input value of the function have to belong to interval [0,1]
     * @return The resulted value as a complex number.
     */
    template<class T>
    std::complex<T> BezierCurve<T>::get_SO_DerivateValue(float input_value){
        T input_value_T=static_cast<T>(input_value);

        return this->SOder_bezierCurve.calculateValue(input_value_T);
    }

    /**
     * @brief Get the polynomial function, which respresents the Bezier curve.
     * 
     */
    template<class T>
    math::PolynomialFunction<std::complex<T>,BEZIER_ORDER> BezierCurve<T>::getBezierCurve(){
        return this->bezierCurve;
    }

    /**
     * @brief Get resulted polynomial function of the first order derivative
     * 
     * @return math::PolynomialFunction<std::complex<T>,BEZIER_ORDER-1> 
     */
    template<class T>
    math::PolynomialFunction<std::complex<T>,BEZIER_ORDER-1> BezierCurve<T>::getFODerivate(){
        return this->FOder_bezierCurve;
    }

    template<class T>
    /**
     * @brief Get resulted polynomial function of the second order derivative
     * 
     * @return math::PolynomialFunction<std::complex<T>,BEZIER_ORDER-2> 
     */
    math::PolynomialFunction<std::complex<T>,BEZIER_ORDER-2> BezierCurve<T>::getSODerivate(){
        return this->SOder_bezierCurve;
    }

    template<class T>
    /**
     * @brief Set the points for creating a new Bezier curve
     * 
     * @param a point A
     * @param b point B
     * @param c point C
     * @param d point D
     */
    void  BezierCurve<T>::setBezierCurve(std::complex<T> a,std::complex<T> b,std::complex<T> c,std::complex<T> d){
        this->points[0]=a;
        this->points[1]=b;
        this->points[2]=c;
        this->points[3]=d;

        this->bezierCurve=this->CP2PF();
        this->FOder_bezierCurve=this->bezierCurve.derivateFO();
        this->SOder_bezierCurve=this->FOder_bezierCurve.derivateFO();
    }

}; // namespace math
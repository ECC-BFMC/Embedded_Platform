template<class T, uint32_t N,uint32_t M>
typename linalg::CMatrix<T,N,M>::CThisType linalg::CMatrix<T,N,M>::inv()
{
    CThisType l_inv(linalg::CLUDecomposition<T,N>(*this).inv());
    return l_inv;
}
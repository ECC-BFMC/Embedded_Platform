#ifndef LINALG_TPP
#define LINALG_TPP

#ifndef LINALG_H
#error __FILE__ should only be included from linalg.h .
#endif // LINALG_H

template<class T, uint32_t N,uint32_t M>
typename linalg::CMatrix<T,N,M>::CThisType linalg::CMatrix<T,N,M>::inv()
{
    CThisType l_inv(linalg::CLUDecomposition<T,N>(*this).inv());
    return l_inv;
}

#endif //LINALG_TPP
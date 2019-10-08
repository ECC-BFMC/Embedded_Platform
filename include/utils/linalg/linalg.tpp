#ifndef LINALG_TPP
#define LINALG_TPP

#ifndef LINALG_H
#error __FILE__ should only be included from linalg.h .
#endif // LINALG_H

template<class T, uint32_t N,uint32_t M>
typename utils::linalg::CMatrix<T,N,M>::CThisType utils::linalg::CMatrix<T,N,M>::inv()
{
    CThisType l_inv(utils::linalg::CLUDecomposition<T,N>(*this).inv());
    return l_inv;
}

#endif //LINALG_TPP
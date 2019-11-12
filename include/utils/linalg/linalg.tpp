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
 */
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
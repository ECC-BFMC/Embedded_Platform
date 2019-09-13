/**
  ******************************************************************************
  * @file    Queue.inl
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class definition for the queue
  *          functionality. Inlin eimplementation.
  ******************************************************************************
 */

/** @brief  Queue Class constructor
 *
 *  Constructor method
 *
 *  
 */
template <class T, unsigned int N>
CQueue<T,N>::CQueue()
    : m_buffer()
    , m_start(0)
    , m_end(N-1)
    , m_size(0)
{
}

/** @brief  Queue Class destructor
 *
 *  Destructor method
 *
 *  
 */
template <class T, unsigned int N>
CQueue<T,N>::~CQueue()
{
}

/** @brief  Is full method
 *
 *  
 *  @return    True if Queue FULL
 */
template <class T, unsigned int N>
bool CQueue<T,N>::isFull()
{
    return (m_start + 1)%N == m_end;
}

/** @brief  Is empty method
 *
 *  
 *  @return    True if Queue EMPTY
 */
template <class T, unsigned int N>
bool CQueue<T,N>::isEmpty()
{
    return (m_end + 1)%N == m_start;
}

/** @brief  Peek method
 *
 *  
 *  @return    Value on top of queue
 */
template <class T, unsigned int N>
T CQueue<T,N>::peek()
{
    return m_buffer[(m_end+1) % N];
}

/** @brief  Pop method
 * 
 *  Method for removing the item on top of the queue and returning it
 *
 *  
 *  @return    Element at top of the queue
 */
template <class T, unsigned int N>
T CQueue<T,N>::pop()
{
    if(!isEmpty())
    {
        m_end = (m_end+1) % N;
        T l_char = m_buffer[m_end];
        m_size--;
        return l_char;
    }
    else
    {
        return 0;
    }
}

/** @brief  Push method
 * 
 *  Method for inserting an item on top of the queue
 *
 *  @param f_char    element to be added to the queue
 *  @return    None
 */
template <class T, unsigned int N>
void CQueue<T,N>::push(T& f_char)
{
    if (!isFull())
    {
        m_buffer[m_start] = f_char;
        m_start = (m_start+1) % N; 
        m_size++;           
    }
}

/** @brief  Push method
 * 
 *  Method for inserting a sequence of items on top of the queue
 *
 *  @param f_char    pointer to element to be added to the queue
 *  @param f_len     number of elements to be added
 *  @return    None
 */
template <class T, unsigned int N>
inline void CQueue<T,N>::push(T *f_char, unsigned int f_len)
{
    for ( unsigned int l_idx = 0; l_idx < f_len; ++l_idx)
    {
        push(f_char[l_idx]);
    }
}

/** @brief  Get size method
 * 
 *  Returns the size of the queue
 *
 *  
 *  @return    Queue size
 */
template <class T, unsigned int N>
unsigned int CQueue<T,N>::getSize()
{
    return m_size;
}

/** @brief  Empty queue method
 *
 *  
 *  @return    None
 */
template <class T, unsigned int N>
inline void CQueue<T,N>::empty()
{
    m_start = 0;
    m_end = N-1;
    m_size = 0;
}
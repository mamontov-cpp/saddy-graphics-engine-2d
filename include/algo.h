/*! \file algo.h
 *
 *  Describes simple algorithms
 */
#pragma once
#include <sadvector.h>
#include <functional>

namespace sad
{

/*! Calls function on each element of vector
 *  \param[in] v vector
 *  \param[in] f function
 */
template<
typename T
>
void each(sad::Vector<T*>& v, const std::function<void(T*)>& f)
{
    for (T* e : v)
    {
        f(e);
    }
}

/*! Calls function on each element of vector
 *  \param[in] v vector
 *  \param[in] f function
 */
template<
    typename T
>
void each(sad::Vector<T>& v, const std::function<void(const T&)>& f)
{
    for (const T& e : v)
    {
        f(e);
    }
}

/*! Calls function on each element of vector
 *  \param[in] v vector
 *  \param[in] f function
 */
template<
    typename T
>
void each(sad::Vector<T>& v, const std::function<void(T&)>& f)
{
    for (T& e : v)
    {
        f(e);
    }
}

}

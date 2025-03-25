/*! \file algo.h
 *
 *  Describes simple algorithms
 */
#pragma once
#include <sadvector.h>
#include <sadpair.h>
#include <algorithm>
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

/*! Normalizes pair as range, making smaller value first, larger second
 *  \param pair used pair
 *  \return result
 */
template<
    typename T
>
sad::Pair<T, T> normalize_range_pair(const sad::Pair<T, T>& pair)
{
    T min = std::min(pair.p1(), pair.p2());
    T max = std::max(pair.p1(), pair.p2());
    return { min ,max };
}

}

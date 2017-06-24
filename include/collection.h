/*! \file collection.h

    A miscellaneous functions for performing actions are stored here
 */
#pragma once
#include <sadvector.h>
#include <functional>
#include <algorithm>

namespace sad
{

/*! Performs action on whole vector
    \param[in] v vector
    \param[in] u unary function
 */
template<
    typename T,
    class UnaryFunction
>
void for_each_whole(sad::Vector<T>& v, const  UnaryFunction& u)
{
    std::for_each(v.begin(), v.end(), u);
}

/*! Invokes functors, stored in array as callback
    \param[in] v vector
 */
template<typename T>
void invoke_functors(sad::Vector<T>& v)
{
    sad::for_each_whole(v, [](T& o) { o(); });
}

}

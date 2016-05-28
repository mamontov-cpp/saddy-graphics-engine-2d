/*! \file collides1d.h
    

    Declares a function for determining, whether two 1-dimensional cutters collide.
 */
#pragma once
#include "../geometry2d.h"

namespace sad
{

namespace p2d
{
/*! Determines, whether two 1-dimensional cutters collide
    \param[in] x11 first point of first cutter
    \param[in] x12 second point of first cutter
    \param[in] x21 first  point of second cutter
    \param[in] x22 second point of second cutter
    \return whether two 1D cutters collide
*/
template<typename _Numeric>
inline bool collides1D(_Numeric x11, _Numeric x12, _Numeric x21, _Numeric x22)
{
    return sad::collides1D<_Numeric>(x11, x12, x21, x22);
}

}

}

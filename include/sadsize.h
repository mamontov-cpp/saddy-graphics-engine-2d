/*! \file sadsize.h
    

    Defines a size class, which contains width and height
 */
#pragma once

namespace sad
{

/*! Defines a size in 2D space, containing width and height.
    Can be used for defining a rectangle or window size, etc.
 */ 
template<
    typename _Type
>
class Size2
{
public:
    /*! Creates size with zero width and zero height
     */
    inline Size2() : Width(0), Height(0)
    {

    }
    /*! Creates a size of specified width and height
        \param[in] width width part of size
        \param[in] height height part of size
     */
    inline Size2(_Type width, _Type height) : Width(width), Height(height)
    {
    }

    _Type Width;  //!< A width, which can be used for defining a width of rectangle
    _Type Height; //!< A height, which can be used for defining a height of rectangle
};

/*! A convenience typedef for floating point size
 */
typedef sad::Size2<double> Size2D;
/*! A convenience typedef for integral size
 */
typedef sad::Size2<unsigned int> Size2I;

}

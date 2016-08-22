/*! \file lengthvalue.h
    
    Describes a length value for layout width or height
 */
#pragma once
#include "unit.h"

namespace sad
{

namespace layouts
{

/*! A length value for width or height
 */	
class LengthValue
{
public:
    /*! A unit value
     */
    sad::layouts::Unit Unit;
    /*! A double value for length
     */
    double Value;

    /*! Constructs a default length value
     */
    LengthValue();
    /*! Constructs new length value
        \param[in] unit a unit for length value
        \param[in] value a value
     */
    LengthValue(sad::layouts::Unit unit, double value);
};
        
}

}
    
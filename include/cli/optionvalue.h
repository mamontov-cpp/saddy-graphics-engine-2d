/*! \file optionvalue.h
    

    Defined an option value for command-line parser
 */
#pragma once
#include "../sadstring.h"
#include "../sadvector.h"

namespace sad
{

namespace cli
{

/*! Defines a value for option, passed to parser
 */
class OptionValue
{
public:
    /*! Whether option was specified explicitly by user
     */
    bool Specified;
    /*! A value for option as flag
     */
    bool Flag;
    /*! A value for option as simple options
     */
    sad::String Single;
    /*! A value for option as multiple options 
     */
    sad::Vector<sad::String> Vector;

    /*! Creates default option value
     */
    OptionValue();
};

}

}

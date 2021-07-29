/*! \file optiontype.h
    

    Declares a type for options in command-line parser
 */
#pragma once

namespace sad
{

namespace cli
{
/*! An option type, which defines a type for an options, passed to parser
 */
enum class OptionType: int
{
    /*!
     * A flag is processed as passing single keyword, like
     * -<flag-name>
     */
    OT_FLAG = 0,
    /*!
     * An option keyword is processed as passing keyword and a value
     * after keyword:
     * -<option-name> <option-value>
     */
    OT_SINGLE = 1,
    /*!
     * An option is processed as passing keyword and several
     * values: -<option-name> <option-value-1> <option-value-2> <option-value-3>
     */
    OT_VECTOR = 2
};

}

}

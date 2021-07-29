/*! \file token.h
    

    Declares a simple token for parsing data
 */
#pragma once
#include "../sadstring.h"

namespace sad
{

namespace cli
{
/*! Describes a token type
 */
enum class TokenType: int
{
    TT_KEYWORD, //!< A token is a keyword
    TT_VALUE,   //!< A token is a value
};
/*! A token as a pair of type and data
 */
class Token
{
 public:
    /*! A type of token
     */
    sad::cli::TokenType Type;
    /*! A data value of token
     */
    sad::String Data;
};

}

}

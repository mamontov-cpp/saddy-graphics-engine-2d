/*! \file pushvalue.h
    \author HiddenSeeker

    Describes an operations, that could be used to push value on a stack of
    operations of Duktape
 */
#pragma once
#include "../sadstring.h"

namespace sad
{

namespace duktape
{

class Context;

/*! Performs pushing value on stack for every type of value
 */
template<
    typename T
>
class PushValue
{
public:
    /*! Performs pushing value 
        \param[in] ctx context
        \param[in] v value
        \param[in] persistent whether value should be in persistent stack 
     */
    static sad::String perform(sad::duktape::Context* ctx, const T& v, bool persistent);
};


#define DEFINE_PUSH_VALUE_SPECIALIZATION( TYPE )   \
template<>										   \
class PushValue< TYPE >                            \
{												   \
public:                                            \
    static sad::String perform(sad::duktape::Context* ctx, TYPE v, bool persistent); \
}; 


DEFINE_PUSH_VALUE_SPECIALIZATION( bool )
DEFINE_PUSH_VALUE_SPECIALIZATION( char )
DEFINE_PUSH_VALUE_SPECIALIZATION( unsigned char )
DEFINE_PUSH_VALUE_SPECIALIZATION( int )
DEFINE_PUSH_VALUE_SPECIALIZATION( float )
DEFINE_PUSH_VALUE_SPECIALIZATION( double )
DEFINE_PUSH_VALUE_SPECIALIZATION( const char* )
DEFINE_PUSH_VALUE_SPECIALIZATION( char* )
DEFINE_PUSH_VALUE_SPECIALIZATION( sad::String )

}

}
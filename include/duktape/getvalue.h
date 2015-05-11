/*! \file getvalue.h
	\author HiddenSeeker

	Describes an operations, that could be used to get value from a stack of
	operations of Duktape
 */
#pragma once
#include "3rdparty/duktape/duktape.h"
#include "pushvalue.h"
#include "../sadstring.h"
#include "../maybe.h"

namespace sad
{

namespace duktape
{

class Context;

/*! Performs getting value from a stack for every type of value
 */
template<
	typename T
>
class GetValue
{
public:
	/*! Performs getting value from stack 
		\param[in] ctx context
		\param[in] pos index for stack
		\return a value if it exists, otherwise empty maybe
	 */
	static sad::Maybe<T> perform(sad::duktape::Context* ctx, duk_idx_t pos);
};


#define DEFINE_GET_VALUE_SPECIALIZATION( TYPE ) template<>					\
class GetValue< TYPE >														\
{																			\
public:																		\
	static sad::Maybe< TYPE > perform(sad::duktape::Context* ctx, duk_idx_t pos);  \
};

DEFINE_GET_VALUE_SPECIALIZATION( bool )
DEFINE_GET_VALUE_SPECIALIZATION( char )
DEFINE_GET_VALUE_SPECIALIZATION( unsigned char )
DEFINE_GET_VALUE_SPECIALIZATION( short )
DEFINE_GET_VALUE_SPECIALIZATION( unsigned short )
DEFINE_GET_VALUE_SPECIALIZATION( int )
DEFINE_GET_VALUE_SPECIALIZATION( unsigned int )
DEFINE_GET_VALUE_SPECIALIZATION( long )
DEFINE_GET_VALUE_SPECIALIZATION( unsigned long )
DEFINE_GET_VALUE_SPECIALIZATION( long long )
DEFINE_GET_VALUE_SPECIALIZATION( unsigned long long )
DEFINE_GET_VALUE_SPECIALIZATION( float )
DEFINE_GET_VALUE_SPECIALIZATION( double )
DEFINE_GET_VALUE_SPECIALIZATION( long double )
DEFINE_GET_VALUE_SPECIALIZATION( const char* )
DEFINE_GET_VALUE_SPECIALIZATION( sad::String )

#undef DEFINE_GET_VALUE_SPECIALIZATION

}

}

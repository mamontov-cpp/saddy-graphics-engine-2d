/*! \file dbtypename.h
	\author HiddenSeeker

	Describes a simple template class for type name as string
 */
#pragma once
#include "../sadstring.h"

namespace sad
{
	
namespace db
{
	
template<
	typename _Type
>
class TypeName
{
public:
	/*! A special name for a type 
	 */
	static sad::String Name;
	/*! Whether type is an object
	 */
	static bool isSadObject;
};

}

}

#ifndef  DECLARE_TYPE_AS_SAD_OBJECT
/*! Declares type compile-time metadata as sad::Object descendant
 */
#define DECLARE_TYPE_AS_SAD_OBJECT(TYPE)                                \
template<> sad::String sad::db::TypeName< TYPE >::Name  = #TYPE;        \
template<> bool sad::db::TypeName< TYPE >::isSadObject  = true;  
#endif

#ifndef  DECLARE_COMMON_TYPE
/*! Declares type compile-time metadata as not a sad::Object descendant
 */
#define DECLARE_COMMON_TYPE(TYPE)                                \
template<> sad::String sad::db::TypeName< TYPE >::Name  = #TYPE;        \
template<> bool sad::db::TypeName< TYPE >::isSadObject  = false;  
#endif

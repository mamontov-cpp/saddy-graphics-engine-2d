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
	/*! A basic name for a pointer type (equal to a Name for a plain type)
	 */
	static sad::String BaseName;
	/*! Whether type is an object (or points to it for pointers)
	 */
	static bool isSadObject;
	/*! Count of pointer stars in type (0 for plain)
	 */
	static const int pointerStarsCount;	
	/*! A default constructor, being called just to make sure, that static fields of class are initialized
	 */
	static inline  void init()
	{
	}
	/*! Defines, whether object cast could be applied
	 */
	enum ObjectCastValueHelper
	{
		CAN_BE_CASTED_TO_OBJECT  = false
	};
};

template<
	typename _Type
>
class TypeName<_Type *>
{
public:
	/*! A special name for a type 
	 */
	static const sad::String Name;
	/*! A basic name for a pointer type (equal to a Name for a plain type)
	 */
	static const sad::String BaseName;
	/*! Whether type is an object (or points to it for pointers)
	 */
	static const bool isSadObject = sad::db::TypeName<_Type>::isSadObject;
	/*! Count of pointer stars in type (0 for plain)
	 */
	static const int pointerStarsCount = sad::db::TypeName<_Type>::pointerStarsCount + 1;
	/*! Defines, whether sad::Object cast could be applied
	 */
	enum ObjectCastValueHelper
	{
		CAN_BE_CASTED_TO_OBJECT  = (sad::TypeName<Type *>::pointerStarsCount == 1)
	};
	/*! A default constructor, being called just to make sure, that static fields of class are initialized
	 */
	static inline void init()
	{
		sad::db::TypeName<_Type *>::Name = sad::db::TypeName<_Type>::Name + sad::String(" *");
		sad::db::TypeName<_Type *>::BaseName = sad::db::TypeName<_Type>::BaseName;
	}

	
};

template<
	typename _Type
>
struct IsSadObject
{
	static const int value = false;
};

}

}

#ifndef  DECLARE_TYPE_AS_SAD_OBJECT_ENUM             
#define DECLARE_TYPE_AS_SAD_OBJECT_ENUM(TYPE)        \
namespace sad { namespace db{ template<> struct IsSadObject< TYPE > { static const int value = true; }; } }; 
#endif

#ifndef  DECLARE_TYPE_AS_SAD_OBJECT
/*! Declares type compile-time metadata as sad::Object descendant
 */
#define DECLARE_TYPE_AS_SAD_OBJECT(TYPE)                                \
template<> sad::String sad::db::TypeName< TYPE >::Name  = #TYPE;        \
template<> bool sad::db::TypeName< TYPE >::isSadObject  = true;         \
template<> sad::String sad::db::TypeName< TYPE >::BaseName  = #TYPE;    \
template<> const int sad::db::TypeName< TYPE >::pointerStarsCount  = 0;         
#endif

#ifndef  DECLARE_COMMON_TYPE
/*! Declares type compile-time metadata as not a sad::Object descendant
 */
#define DECLARE_COMMON_TYPE(TYPE)                                \
template<> sad::String sad::db::TypeName< TYPE >::Name  = #TYPE;        \
template<> bool sad::db::TypeName< TYPE >::isSadObject  = false;        \
template<> sad::String sad::db::TypeName< TYPE >::BaseName  = #TYPE;    \
template<> const int sad::db::TypeName< TYPE >::pointerStarsCount  = 0; 
#endif

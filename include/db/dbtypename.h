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
	static const bool IsSadObject;
	/*! A default constructor, being called just to make sure, that static fields of class are initialized
	 */
	static inline  void init()
	{
	}
	/*! Returns whether base type (without pointer size) is sad::Object descendant
		\return whether type is sad sad::Object descendant
	 */
	static inline bool isSadObject()
	{
		return sad::db::TypeName<_Type>::IsSadObject;
	}

	/*! Returns name for a type name
	 */ 
	static inline sad::String name()
	{
		return  sad::db::TypeName<_Type>::Name;
	}
	/*! Returns base name for a type
	 */
	static inline sad::String baseName()
	{
		return sad::db::TypeName<_Type>::BaseName;
	}
	/*! Defines, whether object cast could be applied
	 */
	enum ObjectCastValueHelper
	{
		CAN_BE_CASTED_TO_OBJECT  = false,
		POINTER_STARS_COUNT = 0
	};
};

template<
	typename _Type
>
class TypeName<_Type *>
{
public:
	/*! Defines, whether sad::Object cast could be applied
	 */
	enum ObjectCastValueHelper
	{
		CAN_BE_CASTED_TO_OBJECT  = (sad::db::TypeName<_Type>::POINTER_STARS_COUNT == 0),
		POINTER_STARS_COUNT = (sad::db::TypeName<_Type>::POINTER_STARS_COUNT + 1),
	};
	/*! A default constructor, being called just to make sure, that static fields of class are initialized
	 */
	static inline void init()
	{

	}
	/*! Returns name for a type name
	 */ 
	static inline sad::String name()
	{
		return  sad::db::TypeName<_Type>::name() + sad::String(" *");
	}
	/*! Returns base name for a type
	 */
	static inline sad::String baseName()
	{
		return sad::db::TypeName<_Type>::BaseName;
	}

	/*! Returns whether base type (without pointer size) is sad::Object descendant
		\return whether type is sad sad::Object descendant
	 */
	static inline bool isSadObject()
	{
		return sad::db::TypeName<_Type>::IsSadObject;
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
template<> const bool sad::db::TypeName< TYPE >::IsSadObject  = true;   \
template<> sad::String sad::db::TypeName< TYPE >::BaseName  = #TYPE;            
#endif

#ifndef  DECLARE_COMMON_TYPE
/*! Declares type compile-time metadata as not a sad::Object descendant
 */
#define DECLARE_COMMON_TYPE(TYPE)                                             \
template<> sad::String sad::db::TypeName< TYPE >::Name  = #TYPE;              \
template<> const bool sad::db::TypeName< TYPE >::IsSadObject  = false;        \
template<> sad::String sad::db::TypeName< TYPE >::BaseName  = #TYPE;          
#endif

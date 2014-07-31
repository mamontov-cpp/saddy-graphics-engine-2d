/*! \file commoncheckedcast.h
	\author HiddenSeeker

	Defines a common checked cast for solving compile-time
	problems with checked_cast if we trying to cast to POD.
 */
#pragma once
#include "../maybe.h"
#include "../sadstring.h"
#include "../classmetadatacontainer.h"
#include "../db/dbtypename.h"

namespace sad
{

class Object;
template<typename _Dest, typename _Src> 
_Dest * checked_cast(_Src * arg);

namespace util
{

/*! Defines a common checked cast for solving compile-time
	problems with checked_cast if we trying to cast to POD.
 */
template<typename T, bool isSadObject>
class CommonCheckedCast
{
public:
	/*! Does nothing, since we don't have a conversion in common cast
		\param[out] result converted value
		\param[in] o object
		\param[in] _typename a typename
	 */
	static void perform(Maybe<T> & v, void * o, const sad::String & _typename)
	{
		
	}
};

/*! Defines a common checked cast case for solving problems with casting to a type.
 */
template<typename T>
class CommonCheckedCast<T, true>
{
public:
	/*! Does nothing, since we don't have a conversion in common cast
		\param[out] result converted value
		\param[in] o object
		\param[in] _typename a typename
	 */
	static void perform(Maybe<T> & result, void * object, const sad::String & _typename)
	{

	}
};

/*! Defines a common checked cast case for solving problems with casting to a type.
 */
template<typename T>
class CommonCheckedCast<T*, true>
{
public:
	/*! Does nothing, since we don't have a conversion in common cast
		\param[out] result converted value
		\param[in] o object
		\param[in] _typename a typename
	 */
	static void perform(Maybe<T*> & result, void * object, const sad::String & _typename)
	{
		bool created = false;
		if (sad::ClassMetaDataContainer::ref()->get(_typename, created)->canBeCastedTo(sad::db::TypeName<T>::Name) == true)
		{
			sad::Object ** o = (sad::Object**)object;
			result.setValue(sad::checked_cast<T, sad::Object>(*o));
		}
	}
};


}

}
/*! \file db/load,h
	\author HiddenSeeker
	
	Describes a load operations for a type
 */
#pragma once
#include "dberror.h"
#include "dbtypename.h"
#include "../object.h"

namespace sad
{

namespace db
{

template<
	typename _Type
>
class Load
{

/*! Load a value of specified type
	\param[in] ptr a value to be saved
	\param[in] v a special value, from which we should load stuff
	\return result
 */
static bool perform(void * ptr, const picojson::value & v)
{
	if (sad::db::TypeName<_Type>::isSadObject)
	{
		return reinterpret_cast<sad::Object *>(ptr)->load(v);
	}
	throw sad::db::NotImplemented("sad::db::Load<_Type>::perform");
	return false;
}

};

}

}

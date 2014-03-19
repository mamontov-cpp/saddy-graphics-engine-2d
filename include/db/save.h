/*! \file db/save,h
	\author HiddenSeeker
	
	Describes a save operations for a type
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
class Save
{

/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (sad::db::TypeName<_Type>::isSadObject)
	{
		return reinterpret_cast<sad::Object *>(ptr)->save();
	}
	throw sad::db::NotImplemented("sad::db::Save<_Type>::perform");
	return picojson::value();
}

};

}

}

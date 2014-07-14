/*! \file dbcanbecasted.h
	\author HiddenSeeker

	Here defined a function,  which determines, whether we could cast 
	between two types in terms of database type and variants.
 */
#pragma once
#include "dbconversiontable.h"

namespace sad
{

namespace db
{

/*! Determines, whether we can cast between two types
	\param[in] from_name a name of type, to cast from
	\param[in] from_is_sad_object whether type to cast from is sad::Object descendant
	\param[in] to_name a name of type, to cast tyo
	\param[in] to_is_sad_object whether type to cast to is sad::Object descendant
 */
inline bool can_be_casted_from_to(const sad::String & from_name, bool from_is_sad_object,
						  const sad::String & to_name, bool to_is_sad_object) 
{
	bool result  = false;
	if (from_is_sad_object && to_is_sad_object)
	{
		bool created = false;
		result = sad::ClassMetaDataContainer::ref()->get(from_name, created)
												   ->canBeCastedTo(to_name);
	}
	else
	{
		result = sad::db::ConversionTable::ref()->converter(from_name, to_name) != NULL;
	}
	return result;
}

/*! Determines, whether we can cast between two types, (type, from which we are casting is declared statically)
	\param[in] to_name a name of type, to cast tyo
	\param[in] to_is_sad_object whether type to cast to is sad::Object descendant
 */
template<
	typename T
>
bool can_be_casted_from_to(const sad::String & to_name, bool to_is_sad_object)
{
	return sad::db::can_be_casted(sad::db::TypeName<T>::Name, sad::db::TypeName<T>::isSadObject,
								  to_name, to_is_sad_object);
}

}

}

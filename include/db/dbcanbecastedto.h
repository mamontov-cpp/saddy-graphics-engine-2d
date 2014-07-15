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
	\param[in] from_base_name a base name of type, to cast from
	\param[in] from_is_sad_object whether type to cast from is sad::Object descendant
	\param[in] from_pointer_star_count a pointer count for the first type
	\param[in] to_base_name a base name of type, to cast to
	\param[in] to_is_sad_object whether type to cast to is sad::Object descendant
	\param[in] to_pointer_star_count a pointer star count for  the second type
 */
inline bool can_be_casted_from_to(
	const sad::String & from_base_name, 
	bool from_is_sad_object,
	int from_pointer_star_count,
	const sad::String & to_base_name, 
	bool to_is_sad_object,
	int to_pointer_star_count
) 
{
	bool result  = false;
	if (from_base_name == to_base_name && from_pointer_star_count == to_pointer_star_count)
		return true;

	if (from_is_sad_object && to_is_sad_object 
		&& from_pointer_star_count == to_pointer_star_count
		&& from_pointer_star_count == 1)
	{
		bool created = false;
		result = sad::ClassMetaDataContainer::ref()->get(from_base_name, created)
												   ->canBeCastedTo(to_base_name);
	}
	else
	{
		if (from_pointer_star_count == to_pointer_star_count && to_pointer_star_count == 0) 
		{
			result = sad::db::ConversionTable::ref()->converter(from_base_name, to_base_name) != NULL;
		}
	}
	return result;
}

/*! Determines, whether we can cast between two types, (type, from which we are casting is declared statically)
	\param[in] to_base_name a base name for type to cast to
	\param[in] to_is_sad_object whether type to cast to is sad::Object descendant
	\param[in] to_pointer_stars_count a count of pointer signs (*) in type
	\return whether we can cast from type T to type to
 */
template<
	typename T
>
bool can_be_casted_from_to(const sad::String & to_base_name, bool to_is_sad_object, int to_pointer_stars_count)
{
	return sad::db::can_be_casted(sad::db::TypeName<T>::BaseName, 
								  sad::db::TypeName<T>::isSadObject,
								  sad::db::TypeName<T>::pointerStarsCount
								  to_base_name, 
								  to_is_sad_object,
								  to_pointer_stars_count
								 );
}

}

}

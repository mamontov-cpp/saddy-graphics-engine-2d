/*! \file dbcanbecastedfromto.h
    

    Here defined a function,  which determines, whether we could cast 
    between two types in terms of database type and variants.
 */
#pragma once
#include "dbtypename.h"

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
bool can_be_casted_from_to(
    const sad::String & from_base_name, 
    bool from_is_sad_object,
    int from_pointer_star_count,
    const sad::String & to_base_name, 
    bool to_is_sad_object,
    int to_pointer_star_count
);

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
    sad::db::TypeName<T>::init();
    return sad::db::can_be_casted_from_to(
        sad::db::TypeName<T>::baseName(), 
        sad::db::TypeName<T>::isSadObject(),
        sad::db::TypeName<T>::POINTER_STARS_COUNT,
        to_base_name, 
        to_is_sad_object,
        to_pointer_stars_count
    );
}

}

}

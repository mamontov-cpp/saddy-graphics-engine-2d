#include "db/dbcanbecastedfromto.h"
#include "db/dbconversiontable.h"
#include "classmetadatacontainer.h"

bool sad::db::can_be_casted_from_to(
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
            result = sad::db::ConversionTable::ref()->converter(from_base_name, to_base_name) != nullptr;
        }
    }
    return result;
}
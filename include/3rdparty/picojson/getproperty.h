/*! \file getproperty.h
    

    A simple helper for getting optional value from picojson::object
 */
#pragma once
#include "picojson.h"
#include "../../maybe.h"
#include "../../sadcolor.h"
#include "../../sadstring.h"

#include <algorithm>

namespace picojson
{

/*! Optionally get property from JSON value
    \param[in] v a value
    \param[in] propertyname
 */
inline  picojson::value const * get_property(
    const picojson::value & v,
    const sad::String & propertyname
)
{
    picojson::value const * result = nullptr;
    if (v.is<picojson::object>())
    {	
        const picojson::object & o  = v.get<picojson::object>();
        picojson::object::const_iterator it = o.find(propertyname);
        if (it != o.end())
        {
            result = &(it->second);
        }
    }
    return result;
}

}

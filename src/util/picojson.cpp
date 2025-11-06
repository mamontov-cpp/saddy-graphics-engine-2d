#include "util/picojson.h"

void sad::util::mergeObjects(picojson::value& outValue, const picojson::value& inValue)
{
    if (outValue.is<picojson::object>() && inValue.is<picojson::object>())
    {
        const picojson::object& obj = inValue.get<picojson::object>();
        for (const auto& kv : obj)
        {
            outValue.insert(kv.first, kv.second);
        }
    }
}
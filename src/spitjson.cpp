#include "spitjson.h"

bool sad::spit(
    const sad::String& fileName, 
    const picojson::value& contents,
    sad::Renderer* renderer
)
{
    return sad::spit(fileName, sad::String(contents.serialize(0)), renderer);
}

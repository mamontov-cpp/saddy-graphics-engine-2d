#include "spitjson.h"

bool sad::spitJson(
    const sad::String& fileName, 
    const picojson::value& contents,
    sad::Renderer* renderer
)
{
    return sad::spit(fileName, sad::String(contents.serialize(0)), renderer);
}

#include "spitjson.h"
#include "opticksupport.h"

bool sad::spitJson(
    const sad::String& fileName, 
    const picojson::value& contents,
    sad::Renderer* renderer
)
{
    PROFILER_EVENT;
    return sad::spit(fileName, sad::String(contents.serialize(0)), renderer);
}

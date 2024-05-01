#include "spit.h"
#include "renderer.h"

#include <fstream>

#include "util/fs.h"
#include "opticksupport.h"

bool sad::spit(
    const sad::String& fileName, 
    const sad::String& contents,
    sad::Renderer* renderer
)
{
    PROFILER_EVENT;
    if (!renderer)
    {
        renderer  = sad::Renderer::ref();
    }

    sad::String realFileName = fileName;
    if (sad::util::isAbsolutePath(fileName) == false)
    {
        realFileName = sad::util::concatPaths(renderer->executablePath(), fileName);
    }

    bool result = false;
    std::ofstream stream(realFileName.c_str());
    if (stream.good())
    {
         stream << contents;
         result = stream.good();
    }
    return result;
}

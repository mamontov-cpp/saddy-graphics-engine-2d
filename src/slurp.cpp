#include "slurp.h"
#include "renderer.h"

#include "util/fs.h"

#include <fstream>



sad::Maybe<sad::String> sad::slurp(const sad::String fileName, sad::Renderer* renderer)
{
    if (!renderer)
    {
        renderer = sad::Renderer::ref();
    }
    sad::Maybe<sad::String> result;
    std::ifstream stream(fileName.c_str());
    if (stream.good())
    {
        std::string alldata(
                (std::istreambuf_iterator<char>(stream)),
                std::istreambuf_iterator<char>()
        );
        result.setValue(alldata);
    }
    else
    {
        if (!sad::util::isAbsolutePath(fileName))
        {
            sad::String newpath = sad::util::concatPaths(renderer->executablePath(), fileName);
            std::ifstream newstream(newpath.c_str());
            if (newstream.good())
            {
                std::string alldata(
                        (std::istreambuf_iterator<char>(newstream)),
                        std::istreambuf_iterator<char>()
                );
                result.setValue(alldata);
            }
        }
    }

    return result;
}

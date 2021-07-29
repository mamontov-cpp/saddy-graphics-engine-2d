/*! \file spitjson.h
    

    Defines function for writing json value into file which should be opened, 
    where executable path is located
 */
#pragma once
#include "spit.h"
#include "3rdparty/picojson/picojson.h"

namespace sad
{
    
/*! Tries to write file into a specified path. If path is relative, tries to write contents
    into a path, relative, to executable path, pointed by renderer. If renderer is nullptr, global
    renderer is used.
    \param[in] fileName name of file, which should be written
    \param[in] contents a contents of file
    \param[in] renderer a renderer, which must be used
    \return true, if data has been written successfully
 */
bool spitJson(
    const sad::String& fileName, 
    const picojson::value& contents,
    sad::Renderer* renderer = nullptr
);

}

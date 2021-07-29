/*! \file spit.h
    

    Defines function for writing string data into file which should be opened, 
    where executable path is located
 */
#pragma once
#include "sadstring.h"

namespace sad
{
    
class Renderer;
/*! Tries to write file into a specified path. If path is relative, tries to write contents
    into a path, relative, to executable path, pointed by renderer. If renderer is nullptr, global
    renderer is used.
    \param[in] fileName name of file, which should be written
    \param[in] contents a contents of file
    \param[in] renderer a renderer, which must be used
    \return true, if data has been written successfully
 */
bool spit(
    const sad::String& fileName, 
    const sad::String& contents,
    sad::Renderer* renderer = nullptr
);

}

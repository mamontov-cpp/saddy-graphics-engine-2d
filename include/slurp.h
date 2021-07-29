/*! \file slurp.h
    

    Defines slurp function for reading whole file into string, using concatenation of given path with 
    executable path, if specified file cannot be found in working directory
 */
#pragma once
#include "sadstring.h"
#include "maybe.h"

namespace sad
{

class Renderer;

/*! Reads file into string if possible, using concatenation of given path with 
    executable path, if specified file cannot be found in working directory.
    \param[in] fileName name of file to be read
    \param[in] renderer used renderer if can be used
    \return a content of file if it can be round
 */
sad::Maybe<sad::String> slurp(const sad::String& fileName, sad::Renderer* renderer = nullptr);

}
/*! \file slurpjson.h
    

    Defines slurp function for reading whole file into json value, using concatenation of given path with 
    executable path, if specified file cannot be found in working directory
 */
#pragma once
#include "slurp.h"
#include "3rdparty/picojson/picojson.h"

namespace sad
{
    
/*! Reads file into picojson value if possible, using concatenation of given path with 
    executable path, if specified file cannot be found in working directory.
    \param[in] fileName name of file to be read
    \param[in] renderer used renderer if can be used
    \return a content of file if it can be round
 */
sad::Maybe<picojson::value> slurpJson(const sad::String fileName, sad::Renderer* renderer = nullptr);

}
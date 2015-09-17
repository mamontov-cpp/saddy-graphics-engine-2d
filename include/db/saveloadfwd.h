/*! \file saveloadfwd.h
    \author HiddenSeeker
    
    A file for forward declaration of save and load callbacks in variant for solving problems with compilation on MinGW.
    Please include db/save.h and db/load.h if you need real implementation.
 */
#pragma once
#include "../3rdparty/picojson/picojson.h"

namespace sad
{

namespace db
{

template<
    typename _Type
>
class Save
{
public:
/*! Saves a value of specified type
    \param[in] ptr a value to be saved
 */
static inline picojson::value perform(void * ptr);

};


template<
    typename _Type
>
class Load
{
public:
/*! Load a value of specified type
    \param[in] ptr a value to be saved
    \param[in] v a special value, from which we should load stuff
    \return result
 */
static bool perform(void * ptr, const picojson::value & v);

};

}

}

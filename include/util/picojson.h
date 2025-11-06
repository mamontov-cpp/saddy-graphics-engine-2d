/*! \file util/picojson.h
 *
 *   An utilities for picojson
 */
#pragma once
#include <3rdparty/picojson/picojson.h>


namespace sad
{

namespace util
{

/*! Merges two picojson objects. Keys in source object cna be replaced by new object
 *  \param[in,out] outValue first value 
 *  \param[in] inValue second value
 */
void mergeObjects(picojson::value& outValue, const picojson::value& inValue);

}

}
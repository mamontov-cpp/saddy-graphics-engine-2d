/*! \file fs.h
    

    Describes a misc functions for working with filesystem
 */
#pragma once
#include "../sadstring.h"

namespace sad
{

namespace util
{

/*! Returns a path delimiter for file system
    \return path delimiter
 */
sad::String pathDelimiter();

/*! Tests whether path is absolute
    \param[in] path a path to resource
    \return whether path is absolute
 */
bool isAbsolutePath(const sad::String & path);

/** Concats two paths for file
    \param[in] parent path
    \param[in] path
    \return parent + path in correct format
 */
sad::String concatPaths(const sad::String & parent,const sad::String & path);

/*! Tries to extract a folder from absolute path
    \param[in] path a path
    \return folder, which file is stored in
 */
sad::String folder(const sad::String & path);

}

}

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

/*! Canonicalizes path for filesystem
    \param[in] path source path for file system
    \return canonicalizedPath
 */
sad::String canonicalizePath(const sad::String& path);

/*! Tests whether path is absolute
    \param[in] path a path to resource
    \return whether path is absolute
 */
bool isAbsolutePath(const sad::String & path);

/** Concatenates two paths for file
    \param[in] parent path
    \param[in] path child path
    \return parent + path in correct format
 */
sad::String concatPaths(const sad::String & parent,const sad::String & path);

/*! Tries to extract a folder from absolute path
    \param[in] path a path
    \return folder, which file is stored in
 */
sad::String folder(const sad::String & path);

/*! Returns true if file exists
    \param[in] path a path for file
    \return true if exists
 */
bool fileExists(const char* path);

}

}

/*! \file loader.h
    

    Defines a loader, which can load a texture from file stream,
    using specified format.
 */
#pragma once
#include "../texture.h"
#include <cstdio>

namespace tar7z
{
class Entry;
}

namespace sad
{

namespace imageformats
{
/*! Defines a loader, which can load a texture from file stream,
    using specified format.
 */
class Loader
{
public:
    /*! Loads a texture from file stream. file must be opened in binary format for reading
        \param[in] file a file
        \param[in] texture a texture
        \return true on success
     */
    virtual bool load(FILE * file, sad::Texture * texture) = 0;
    /*! Loads texture from archive entry.
        \param[in] entry a file entry to be loaded
        \param[in] texture a source texture
     */
    virtual bool load(tar7z::Entry* entry, sad::Texture* texture);
    /*! Kept for purpose of inheritance
     */
    virtual ~Loader();
};

}

}

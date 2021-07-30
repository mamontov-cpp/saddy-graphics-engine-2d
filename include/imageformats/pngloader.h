/*! \file pngloader.h
    

    Defines a loader for PNG images, using PicoPNG
 */
#pragma once
#include "loader.h"

namespace sad
{

namespace imageformats
{
/*! Defines a loader for PNG file format
 */
class PNGLoader: public sad::imageformats::Loader
{
public:
    /*! Loads a texture from file stream. file must be opened in binary format for reading
        \param[in] file a file to be read
        \param[out] texture a texture
        \return true on success
     */
    virtual bool load(FILE * file, sad::Texture * texture) override;
    /*! Loads texture from archive entry.
        \param[in] entry a file entry to be loaded
        \param[out] texture a source texture
     */
    virtual bool load(tar7z::Entry* entry, sad::Texture* texture) override;
    /*! Kept for purpose of inheritance
     */
    virtual ~PNGLoader() override;
};

}

}

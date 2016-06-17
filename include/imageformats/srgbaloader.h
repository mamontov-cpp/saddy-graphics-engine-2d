/*! \file srgbaloader.h
    

    Defines a loader for SRGBA images. 
    See https://github.com/mamontov-cpp/saddy-graphics-engine-2d/issues/82 for details.
    See https://github.com/mamontov-cpp/saddy-graphics-engine-2d/wiki/T160.-(1.6.0-and-later)-Simple-image-format for a format description.
 */
#pragma once
#include "loader.h"

namespace sad
{

namespace imageformats
{
/*! Defines a loader for SRGBA file format.
    See https://github.com/mamontov-cpp/saddy-graphics-engine-2d/issues/82 for details.
    See https://github.com/mamontov-cpp/saddy-graphics-engine-2d/wiki/T160.-(1.6.0-and-later)-Simple-image-format for a format description.
 */
class SRGBALoader: public sad::imageformats::Loader
{
public:
    /*! Loads a texture from file stream. file must be opened in binary format for reading
        \param[in] file
        \param[in] texture
        \return true on success
     */
    virtual bool load(FILE * file, sad::Texture * texture);
    /*! Kept for purpose of inheritance
     */
    virtual ~SRGBALoader();
};

}

}

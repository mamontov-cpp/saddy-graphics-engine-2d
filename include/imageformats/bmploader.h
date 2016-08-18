/*! \file bmploader.h
    

    Defines a loader for BMP images
 */
#pragma once
#include "loader.h"

#include <streambuf>

namespace sad
{

namespace imageformats
{
/*! Defines a loader for BMP file format
 */
class BMPLoader: public sad::imageformats::Loader
{
public:
    /*! Loads a texture from file stream. file must be opened in binary format for reading
        \param[in] file
        \param[in] texture
        \return true on success
     */
    virtual bool load(FILE * file, sad::Texture * texture);
    /*! Loads texture from archive entry.
        \param[in] entry a file entry to be loaded
        \param[in] texture a source texture
     */
    virtual bool load(tar7z::Entry* entry, sad::Texture* texture);	
    /*! Kept for purpose of inheritance
     */
    virtual ~BMPLoader();
protected:
    /*! Loads a texture from readable buffer
        \param[in] file
        \param[in] texture
        \return true on success
     */
    virtual bool load(std::streambuf * file, sad::Texture * texture);
};

}

}

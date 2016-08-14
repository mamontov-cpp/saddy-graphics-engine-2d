/*! \file tgaloader.h
    

    Defines a loader for TGA images
 */
#pragma once
#include "loader.h"

namespace sad
{

namespace imageformats
{
/*! Defines a loader for TGA file format
 */
class TGALoader: public sad::imageformats::Loader
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
    virtual ~TGALoader();
protected:
    /*! Flips texture horizontally and vertically, according to
        flaps, set while loading
        \param[in] texture a texture in loader
     */
    void flip(sad::Texture * texture) const;
    /*! Loads raw uncompressed image
        \param[in] texture a texture
        \return whether loading was successfull
     */
    bool loadRaw(sad::Texture * texture) const;
    /*! Loads run-length encoded image
        \param[in] texture a texture
        \return whether loading was successfull
     */
    bool loadCompressed(sad::Texture * texture) const;
    /*! Whether we should flip image horizontally.
        This flag is set in sad::imageformats::TGALoader::load.
     */
    bool m_flip_x;
    /*! Whether we should flip image vertically
        This flag is set in sad::imageformats::TGALoader::load.
     */
    bool m_flip_y;
    /* Amount of bytes (not bits) per pixel
        This field is set in sad::imageformats::TGALoader::load.	
     */
    unsigned int m_bypp;
    /*! An opened file stream
        This field is set in sad::imageformats::TGALoader::load.	
     */
    FILE * m_file;
};

}

}

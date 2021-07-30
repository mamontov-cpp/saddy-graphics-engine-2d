/*! \file tgaloader.h
    

    Defines a loader for TGA images
 */
#pragma once
#include "loader.h"

#include <istream>

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
        \param[in] file a file
        \param[in] texture a texture
        \return true on success
     */
    virtual bool load(FILE * file, sad::Texture * texture) override;
    /*! Loads texture from archive entry.
        \param[in] entry a file entry to be loaded
        \param[in] texture a source texture
     */
    virtual bool load(tar7z::Entry* entry, sad::Texture* texture) override;
    /*! Kept for purpose of inheritance
     */
    virtual ~TGALoader() override;
protected:
    /*! Flips texture horizontally and vertically, according to
        flaps, set while loading
        \param[in] texture a texture in loader
     */
    void flip(sad::Texture * texture) const;
    /*! Tries to load a texture from a current stream
        \param[in] texture a texture
     */
    bool load(sad::Texture* texture);
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
    std::istream * m_file;
};

}

}

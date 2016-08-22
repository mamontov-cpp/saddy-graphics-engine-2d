/*! \file pixelstorageloader.h
    

    Defines a loader for primitive pixels, stored as plain buffer, preceded by size byte and signature.
    A basic example of this format is SRGBA. 

    See https://github.com/mamontov-cpp/saddy-graphics-engine-2d/issues/82 , https://github.com/mamontov-cpp/saddy-graphics-engine-2d/issues/94 for details.

    See https://github.com/mamontov-cpp/saddy-graphics-engine-2d/wiki/T160.-(1.6.0-and-later)-Simple-image-format for a SRGBA format description.
 */
#pragma once
#include "loader.h"

namespace sad
{

namespace imageformats
{
/*! Defines a loader for primitive pixels, stored as plain buffer, preceded by size byte and signature.

 */
class PixelStorageLoader: public sad::imageformats::Loader
{
public:
/*! A settings for a loader, which define, how specific format loaded will work
 */ 
struct Settings
{
    /*! Must point into buffer, where ethalon signature is defined
     */
    const unsigned char* Signature;
    /*!  Contains a size of signature
     */
    size_t SignatureSize;
    /*! Amount of bits per pixel in loaded texture
     */
    size_t Bpp;
    /*! A format, which texture must have after loading image. Must be a value from sad::Texture::Format 
     */
    unsigned int Format;
    /*! Constructs new settings
        \param[in] s signature
        \param[in] ssize signature size
        \param[in] bpp bits per pixel in loaded texture
        \param[in] fmt format format of loaded texture
     */
    inline Settings(const unsigned char* s, size_t ssize, size_t bpp, unsigned int fmt) : Signature(s), SignatureSize(ssize), Bpp(bpp), Format(fmt)
    {
    }
};
    /*! Makes new loader with specified settings
        \param[in] settings a settings
     */
    PixelStorageLoader(const sad::imageformats::PixelStorageLoader::Settings& settings);
    /*! Loads a texture from file stream. File must be opened in binary format for reading.
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
    virtual ~PixelStorageLoader();
    /*! A settings for SRGBA format
     */
    static sad::imageformats::PixelStorageLoader::Settings SRGBASettings;
    /*! A settings for SR5G6B5 format
     */
    static sad::imageformats::PixelStorageLoader::Settings SR5G6B5Settings;
    /*! A settings for SR4G4B4A4 format
     */
    static sad::imageformats::PixelStorageLoader::Settings SR4G4B4A4Settings;
    /*! A settings for SR3G3B2 format
     */
    static sad::imageformats::PixelStorageLoader::Settings SR3G3B2Settings;
protected:
    /*! A settings for loader
     */
    sad::imageformats::PixelStorageLoader::Settings m_settings;
};

}

}

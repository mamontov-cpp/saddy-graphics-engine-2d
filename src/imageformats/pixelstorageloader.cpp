#include "imageformats/pixelstorageloader.h"

#include "texture.h"

#define TAR7Z_SADDY

#include "3rdparty/tar7z/include/tar.h"

const int maxlogtexturesize = 14;

sad::imageformats::PixelStorageLoader::PixelStorageLoader(const sad::imageformats::PixelStorageLoader::Settings& settings) : m_settings(settings)
{

}

bool sad::imageformats::PixelStorageLoader::load(FILE * file, sad::Texture * texture)
{
    // Exit on invalid input data
    if (!file || !texture)
    {
        return false;
    }

    const int headersize = m_settings.SignatureSize + 1;
    sad::uchar* header = new sad::uchar[headersize];

    // Exit if unable to read header
    if (fread(header, headersize, 1, file) != 1)
    {
        delete[] header;
        return false;
    }

    // Exit if signature is invalid
    if (memcmp(header, m_settings.Signature, m_settings.SignatureSize) != 0)
    {
        delete[] header;
        return false;
    }


    // Exit if image size is too large
    sad::uchar logtexsize = header[headersize - 1];
    if (logtexsize > maxlogtexturesize)
    {
        delete[] header;
        return false;
    }
    delete[] header;

    unsigned int texsize = 1 << static_cast<unsigned int>(logtexsize);
    unsigned int buffersize = texsize * texsize * (m_settings.Bpp / 8);

    sad::Texture::DefaultBuffer* newbuffer = new sad::Texture::DefaultBuffer();
    newbuffer->Data.resize(buffersize);

    sad::uchar* buffer = &(newbuffer->Data[0]);
    if (fread(buffer, buffersize, 1, file) != 1)
    {
        return false;
    }
    texture->width() = texsize;
    texture->height() = texsize;
    texture->bpp() = m_settings.Bpp;
    texture->Format = static_cast<sad::Texture::InternalFormat>(m_settings.Format);
    delete texture->Buffer;
    texture->Buffer = newbuffer;

    return true;
}

bool sad::imageformats::PixelStorageLoader::load(tar7z::Entry* entry, sad::Texture* texture)
{
    if (entry == nullptr || texture == nullptr)
        return false;

    const char* buffer = entry->contents();

    const int headersize = m_settings.SignatureSize + 1;

    if (entry->Size < headersize)
    {
        return false;
    }

    // Exit if signature is invalid
    if (memcmp(buffer, m_settings.Signature, m_settings.SignatureSize) != 0)
    {
        return false;
    }


    // Exit if image size is too large
    sad::uchar logtexsize = buffer[headersize - 1];
    if (logtexsize > maxlogtexturesize)
    {
        return false;
    }

    unsigned int texsize = 1 << static_cast<unsigned int>(logtexsize);
    unsigned int buffersize = texsize * texsize * (m_settings.Bpp / 8);

    // Exit on insufficient space
    if (buffersize + headersize > entry->Size)
    {
        return false;
    }

    texture->width() = texsize;
    texture->height() = texsize;
    texture->bpp() = m_settings.Bpp;
    texture->Format = static_cast<sad::Texture::InternalFormat>(m_settings.Format);
    delete texture->Buffer;
    sad::Texture::Tar7zArchiveBuffer* buf = new sad::Texture::Tar7zArchiveBuffer();
    buf->Archive = entry->Parent;
    buf->Offset = entry->Offset + headersize;
    texture->Buffer = buf;

    return true;
}

sad::imageformats::PixelStorageLoader::~PixelStorageLoader()
{

}

const sad::uchar SRGBAsignature[]     =  {'S', 'R', 'G', 'B', 'A'};

const sad::uchar SR5G6B5signature[]   =  {'S', 'R', '5', 'G', '6', 'B', '5'};

const sad::uchar SR4G4B4A4signature[] =  {'S', 'R', '4', 'G', '4', 'B', '4', 'A', '4'};

const sad::uchar SR3G3B2signature[]   =  {'S', 'R', '3', 'G', '3', 'B', '2'};

sad::imageformats::PixelStorageLoader::Settings sad::imageformats::PixelStorageLoader::SRGBASettings(SRGBAsignature, 5, 32, sad::Texture::SFT_R8_G8_B8_A8);
sad::imageformats::PixelStorageLoader::Settings sad::imageformats::PixelStorageLoader::SR5G6B5Settings(SR5G6B5signature, 7, 16, sad::Texture::SFT_R5_G6_B5);
sad::imageformats::PixelStorageLoader::Settings sad::imageformats::PixelStorageLoader::SR4G4B4A4Settings(SR4G4B4A4signature, 9, 16, sad::Texture::SFT_R4_G4_B4_A4);
sad::imageformats::PixelStorageLoader::Settings sad::imageformats::PixelStorageLoader::SR3G3B2Settings(SR3G3B2signature, 7, 8, sad::Texture::SFT_R3_G3_B2);

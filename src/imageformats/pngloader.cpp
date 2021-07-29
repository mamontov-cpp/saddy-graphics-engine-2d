#include "imageformats/pngloader.h"

#define TAR7Z_SADDY

#include "3rdparty/tar7z/include/tar.h"

#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION
#include "3rdparty/stb/stb_image.h"


bool sad::imageformats::PNGLoader::load(FILE * file, sad::Texture * texture)
{
    if (file == nullptr || texture == nullptr)
        return false;

    int components = 4;
    int width = 0, height = 0;
    unsigned char* data = stbi_load_from_file(file, &width, &height, &components, 4);
    
    bool ok = data != nullptr;
    if (ok)
    {
        texture->width() = static_cast<unsigned int>(width);
        texture->height() = static_cast<unsigned int>(height);
        texture->bpp() = 32;
        texture->Format = sad::Texture::InternalFormat::SFT_R8_G8_B8_A8;
        delete texture->Buffer;
        texture->Buffer = new sad::Texture::PointerBuffer(data);        
    }
    return ok;
}

bool sad::imageformats::PNGLoader::load(tar7z::Entry* entry, sad::Texture* texture)
{
    if (entry == nullptr || texture == nullptr)
        return false;

    int components = 4;
    int width = 0, height = 0;
    unsigned char* data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(entry->contents()), entry->Size, &width, &height, &components, 4);

    bool ok = data != nullptr;
    if (ok)
    {
        texture->width() = static_cast<unsigned int>(width);
        texture->height() = static_cast<unsigned int>(height);
        texture->bpp() = 32;
        texture->Format = sad::Texture::InternalFormat::SFT_R8_G8_B8_A8;
        delete texture->Buffer;
        texture->Buffer = new sad::Texture::PointerBuffer(data);
    }
    return ok;
}

sad::imageformats::PNGLoader::~PNGLoader()
{

}

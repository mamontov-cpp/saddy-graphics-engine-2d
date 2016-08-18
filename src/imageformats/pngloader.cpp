#include "imageformats/pngloader.h"

#define TAR7Z_SADDY

#include "3rdparty/tar7z/include/tar.h"

int decodePNG(std::vector<unsigned char>& out_image, unsigned long& image_width, unsigned long& image_height, const unsigned char* in_png, size_t in_size, bool convert_to_rgba32 = true);

bool sad::imageformats::PNGLoader::load(FILE * file, sad::Texture * texture)
{
    if (file == NULL || texture == NULL)
        return false;

    std::vector<unsigned char> buffer;
    fseek(file, 0L, SEEK_END);
    unsigned int size = ftell(file);
    fseek(file, 0L, SEEK_SET);
    buffer.resize(static_cast<size_t>(size));
    size_t readbytes = fread(reinterpret_cast<char*>(&buffer[0]), 1, size, file);
    if (readbytes != size)
    {
        buffer.clear();
        return false;
    }
    
    unsigned long width = 0;
    unsigned long height = 0;

    sad::Texture::DefaultBuffer* newbuffer = new sad::Texture::DefaultBuffer();

    int result = decodePNG(
        newbuffer->Data,
        width,
        height,
        &(buffer[0]), 
        buffer.size()
    );
    
    bool ok = result == 0;
    if (ok)
    {
        texture->width() = width;
        texture->height() = height; 
        texture->bpp() = 32;
        texture->Format = sad::Texture::SFT_R8_G8_B8_A8;
        delete texture->Buffer;
        texture->Buffer = newbuffer;        
    }
    else
    {
        delete newbuffer;
    }
    return ok;
}

bool sad::imageformats::PNGLoader::load(tar7z::Entry* entry, sad::Texture* texture)
{
    if (entry == NULL || texture == NULL)
        return false;

    unsigned long width = 0;
    unsigned long height = 0;

    sad::Texture::DefaultBuffer* newbuffer = new sad::Texture::DefaultBuffer();

    int result = decodePNG(
        newbuffer->Data,
        width,
        height,
        reinterpret_cast<unsigned char*>(entry->contents()), 
        entry->Size
    );
    
    bool ok = result == 0;
    if (ok)
    {
        texture->width() = width;
        texture->height() = height; 
        texture->bpp() = 32;
        texture->Format = sad::Texture::SFT_R8_G8_B8_A8;
        delete texture->Buffer;
        texture->Buffer = newbuffer;        
    }
    else
    {
        delete newbuffer;
    }
    return ok;
}

sad::imageformats::PNGLoader::~PNGLoader()
{

}

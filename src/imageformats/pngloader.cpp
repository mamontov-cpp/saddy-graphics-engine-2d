#include "imageformats/pngloader.h"

int decodePNG(std::vector<unsigned char>& out_image, unsigned long& image_width, unsigned long& image_height, const unsigned char* in_png, size_t in_size, bool convert_to_rgba32 = true);

bool sad::imageformats::PNGLoader::load(FILE * file, sad::Texture * texture)
{
    if (file == NULL || texture == NULL)
        return false;

    std::vector<unsigned char> buffer;
    fseek(file, 0L, SEEK_END);
    unsigned int size = ftell(file);
    fseek(file, 0L, SEEK_SET);
    buffer.resize((size_t)size);
    size_t readbytes = fread((char*)(&buffer[0]), 1, size, file);
    if (readbytes != size)
    {
        buffer.clear();
        return false;
    }
    
    std::vector<unsigned char> outimg;
    unsigned long width = 0;
    unsigned long height = 0;

    int result = decodePNG(
        texture->vdata(),
        width,
        height,
        &(buffer[0]), 
        buffer.size()
    );
    texture->width() = width;
    texture->height() = height; 
    texture->bpp() = 32;
    
    bool ok = result == 0;
    return ok;
}

sad::imageformats::PNGLoader::~PNGLoader()
{

}

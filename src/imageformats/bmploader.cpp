#include "imageformats/bmploader.h"

#include <iostream>


#include "util/chararrayibuf.h"
#include "util/fileistreambuf.h"

#define TAR7Z_SADDY

#include "3rdparty/tar7z/include/tar.h"

#pragma pack(push, 1)

namespace sad
{

namespace imageformats
{

struct BMPHeader
{
    unsigned short type;
    unsigned int  size;
    unsigned short reserv1;
    unsigned short reserv2;
    unsigned int  offsetbits;
};

struct BMPImageHeader
{
    unsigned int size;
    unsigned int width;
    unsigned int height;
    unsigned short planes;
    unsigned short bitcount;
    unsigned int  compression;
    unsigned int  sizeimage;
    int           xpels;
    int           ypels;
    unsigned int  cused;
    unsigned int  cimportant;
};

}

}

#pragma pack(pop)

static const short bmp_magic_number = 19778;

// ========================================================================== PUBLIC METHODS ==========================================================================

bool sad::imageformats::BMPLoader::load(FILE * file, sad::Texture * texture)
{
    if (file == nullptr || texture == nullptr)
        return false;

    sad::util::FileIStreamBuf buf(file);
    return this->load(&buf, texture);
}

bool sad::imageformats::BMPLoader::load(tar7z::Entry* entry, sad::Texture* texture)
{
    if (entry == nullptr || texture == nullptr)
    {
        return false;
    }
    sad::util::CharArrayIBuf buf(entry->contents(), entry->contents() + entry->Size);
    return this->load(&buf, texture);
}

sad::imageformats::BMPLoader::~BMPLoader()
{

}

// ========================================================================== PROTECTED METHODS ==========================================================================

bool sad::imageformats::BMPLoader::load(std::streambuf* buf, sad::Texture* texture)
{
    if (buf == nullptr || texture == nullptr)
        return false;

    std::istream stream(buf);
    // Try to read header
    sad::imageformats::BMPHeader header;

    stream.read(reinterpret_cast<char*>(&header), sizeof(sad::imageformats::BMPHeader));
    if (stream.fail() || stream.eof())
    {
        return false;
    }

    // Check for magic number
    if (header.type != bmp_magic_number)
    {
        return false;
    }

    // Read image information
    sad::imageformats::BMPImageHeader image_header;
    stream.read(reinterpret_cast<char*>(&image_header), sizeof(sad::imageformats::BMPImageHeader));
    if (stream.fail() || stream.eof())
    {
        return false;
    }

    // Check whether image is supported
    bool image_unsupported = (image_header.bitcount != 24 && image_header.bitcount != 32);
    if (image_header.width == 0 || image_header.height == 0 || image_unsupported)
    {
        return false;
    }

    // Try to move to image data beginning to read it
    stream.seekg(header.offsetbits, std::ios_base::beg);
    if (stream.fail() || stream.eof())
    {
        return false;
    }

    unsigned char bypp = image_header.bitcount / 8;
    
    unsigned int width = image_header.width;
    unsigned int fileimagesize = image_header.width * image_header.height;  
    
    // Buffer
    sad::Texture::DefaultBuffer* newbuffer = new sad::Texture::DefaultBuffer();
    newbuffer->Data.resize(fileimagesize * 4, 255);

    int x = 0;
    int y = image_header.height - 1;
    
    sad::uchar * buffer = &(newbuffer->Data[0]);
    for (unsigned long i = 0; i < fileimagesize; i++)
    {
        unsigned int offset = 4 * (y * width + x);
        unsigned char * offset_buffer = buffer + offset;

        stream.read(reinterpret_cast<char*>(offset_buffer), bypp);
        if (stream.fail() || stream.eof())
        {
            delete newbuffer;
            return false;
        }
        
        std::swap(*offset_buffer, *(offset_buffer+2));
                
        x = x + 1;
        if (x == width)
        {
            y -= 1;
            x =  0;
        }
    }

    // Much better to do this in case
    texture->width() = width;
    texture->height() = image_header.height;
    texture->bpp() = 32;
    texture->Format = sad::Texture::InternalFormat::SFT_R8_G8_B8_A8;
    delete texture->Buffer;
    texture->Buffer = newbuffer;
    return true;
}

std::vector<unsigned char> sad::imageformats::dumpToBMP(unsigned int width, unsigned int height, int components, void* texture)
{
    std::vector<unsigned char> result;
    if ((components != 3) && (components != 4))
    {
        throw std::logic_error("Error! Invalid amount of components.");
    }
    unsigned int pad_size =  4 - ((width * 3) % 4);
    if (pad_size == 4)
    {
        pad_size = 0;
    }
    unsigned int padded_width = width + pad_size;
    unsigned int image_size = (width + padded_width) * height * 3;
    unsigned int full_file_size  = sizeof(sad::imageformats::BMPHeader) + sizeof(sad::imageformats::BMPImageHeader) + image_size;
    result.resize(full_file_size);
    unsigned char* pointer_to_start = result.data();
    sad::imageformats::BMPHeader* bmp_header = reinterpret_cast<sad::imageformats::BMPHeader*>(pointer_to_start);
    bmp_header->type = bmp_magic_number;
    bmp_header->size = full_file_size;
    bmp_header->reserv1 = 0;
    bmp_header->reserv2 = 0;
    bmp_header->offsetbits = sizeof(sad::imageformats::BMPHeader) + sizeof(sad::imageformats::BMPImageHeader); // Offset of two headers

    sad::imageformats::BMPImageHeader* bmp_image_header = reinterpret_cast<sad::imageformats::BMPImageHeader*>(pointer_to_start + sizeof(sad::imageformats::BMPHeader));
    bmp_image_header->size = sizeof(sad::imageformats::BMPImageHeader);
    bmp_image_header->width = width;
    bmp_image_header->height = height;
    bmp_image_header->planes = 0;
    bmp_image_header->bitcount = 3 * 8; // RGB
    bmp_image_header->compression = 0;
    bmp_image_header->sizeimage = image_size;
    bmp_image_header->xpels = 0;
    bmp_image_header->ypels = 0;
    bmp_image_header->cused = 0;
    bmp_image_header->cimportant = 0;

    unsigned char* current = pointer_to_start + sizeof(sad::imageformats::BMPHeader) + sizeof(sad::imageformats::BMPImageHeader);
    unsigned char* current_texture_pointer = reinterpret_cast<unsigned char*>(texture);
    for(int y = height - 1; y > -1; y--)
    {
        for(int x = 0; x < width; x++) {
            unsigned char* pixel_begin = current_texture_pointer + (y * width + x) * components;
            current[0] = pixel_begin[2];
            current[1] = pixel_begin[1];
            current[2] = pixel_begin[0];
            //current[3] = 0;
            current += 3;
        }
        for(size_t i = 0; i < pad_size; i++)
        {
            *current = 0;
            current += 1;
        }
    }

    return result;
}

bool sad::imageformats::dumpToBMP(const std::string& file_name, unsigned int width, unsigned int height, int components, void* texture)
{
    std::vector<unsigned char> file_data = sad::imageformats::dumpToBMP(width, height, components, texture);
    FILE* file = fopen(file_name.c_str(), "wb");
    if (!file)
    {
        return false;
    }
    bool ok = fwrite(&(file_data[0]),1, file_data.size(), file) != file_data.size();
    fclose(file);
    return ok;
}
#include "imageformats/tgaloader.h"

#include "util/chararrayibuf.h"
#include "util/fileistreambuf.h"

#define TAR7Z_SADDY

#include "3rdparty/tar7z/include/tar.h"

#ifdef TEXTURE_LOADER_TEST
    #include <cstdio>
#endif

namespace sad
{

namespace imageformats
{

enum class ImageType: int
{
    IT_NO_DATA        = 0,      /*!< Image doesn't contains any data.                    */
    IT_COLORMAP       = 1,      /*!< Color map without RLE compression.                  */
    IT_TRUECOLOR      = 2,      /*!< RGB image without RLE compression.                  */
    IT_MONOCHROME     = 3,      /*!< Monochrome image without RLE compression.           */
    IT_COLORMAP_RLE   = 9,      /*!< Color map with RLE compression.                     */
    IT_TRUECOLOR_RLE  = 10,     /*!< RGB image with RLE compression.                     */
    IT_MONOCHROME_RLE = 11      /*!< Monochrome image with RLE compression.              */
};

struct TGAHeader
{
    unsigned char  idLength;            /*!< Length of ID field.                        (1 byte) */
    unsigned char  colorMapType;        /*!< Type of color map.                         (1 byte) */
    unsigned char  imageType;           /*!< type of image.                             (1 byte) */

    unsigned short colorMapOrigin;      /*!< First entry of color map.                  (2 byte) */
    unsigned short colorMapSize;        /*!< Length of color map.                       (2 byte) */
    unsigned char  colorMapEntrySize;   /*!< Size of color map.                         (1 byte) */

    unsigned short XOrigin;             /*!< Horizontal coordinate of image's origin.   (2 byte) */
    unsigned short YOrigin;             /*!< Vertical coordinate of image's origin.     (2 byte) */
    unsigned short width;               /*!< Width of the image.                        (2 byte) */
    unsigned short height;              /*!< Height of the image.                       (2 byte) */
    unsigned char  bitsPerPix;          /*!< Bits per pixel.                            (1 byte) */
    unsigned char  imageDescriptor;     /*!< Bits of image descriptor.                  (1 byte) */
};

}

}

static const unsigned int  supported_image_types_count = 2;
static const unsigned char supported_image_types[supported_image_types_count] = { 
    static_cast<int>(sad::imageformats::ImageType::IT_TRUECOLOR), 
    static_cast<int>(sad::imageformats::ImageType::IT_TRUECOLOR_RLE) 
};

bool sad::imageformats::TGALoader::load(FILE * file, sad::Texture * texture)
{
    if (file == nullptr || texture == nullptr)
    {
        return false;
    }
    sad::util::FileIStreamBuf buf(file);
    std::istream stream(&buf);
    m_file = &stream;
    bool result = this->load(texture);
    m_file = nullptr;
    return result;
}

bool sad::imageformats::TGALoader::load(tar7z::Entry* entry, sad::Texture* texture)
{
    if (entry == nullptr || texture == nullptr)
    {
        return false;
    }
    sad::util::CharArrayIBuf buf(entry->contents(), entry->contents() + entry->Size);
    std::istream stream(&buf);
    m_file = &stream;
    bool result =  this->load(texture);
    m_file = nullptr;
    return result;
}

sad::imageformats::TGALoader::~TGALoader() = default;

bool sad::imageformats::TGALoader::load(sad::Texture* texture)
{
    /*! Try to read the tga header
     */
    const unsigned int header_buffer_size = 18;
    sad::uchar header_buffer[header_buffer_size];
    m_file->read(reinterpret_cast<char*>(header_buffer), header_buffer_size * sizeof(sad::uchar));
    if (m_file->fail() || m_file->eof())
    {
        return false;
    }

    /*! Decode header
     */ 
    sad::imageformats::TGAHeader header;
    header.idLength         = header_buffer[0];
    header.colorMapType     = header_buffer[1];
    header.imageType        = header_buffer[2];
    header.colorMapOrigin   = header_buffer[3]  + 256 * header_buffer[4];
    header.colorMapSize     = header_buffer[5]  + 256 * header_buffer[6];
    header.colorMapEntrySize= header_buffer[7];
    header.XOrigin          = header_buffer[8]  + 256 * header_buffer[9];
    header.YOrigin          = header_buffer[10] + 256 * header_buffer[11];
    header.width            = header_buffer[12] + 256 * header_buffer[13];
    header.height           = header_buffer[14] + 256 * header_buffer[15];
    header.bitsPerPix       = header_buffer[16];
    header.imageDescriptor  = header_buffer[17];

    bool image_unsupported = header.imageType != static_cast<int>(sad::imageformats::ImageType::IT_TRUECOLOR)
                          && header.imageType != static_cast<int>(sad::imageformats::ImageType::IT_TRUECOLOR_RLE);
    if (header.width == 0 || header.height == 0 || image_unsupported)
    {
        return false;
    }

    sad::Texture::DefaultBuffer* buffer = new sad::Texture::DefaultBuffer();
    delete texture->Buffer;
    texture->Buffer = buffer;
    buffer->Data.clear();
    // Resize to ARGB size and set texture properties
    texture->width() = header.width;
    texture->height() = header.height;
    texture->bpp() = 32;
    texture->Format = sad::Texture::InternalFormat::SFT_R8_G8_B8_A8;
    buffer->Data.resize(header.width * header.height * 4, 255);
    
    // Set bytes per pixel
    m_bypp = header.bitsPerPix / 8;
    bool result = false;
    if (header.imageType == static_cast<int>(ImageType::IT_TRUECOLOR_RLE)) 
    {
        result = loadCompressed(texture);
    }
    if ( header.imageType == static_cast<int>(ImageType::IT_TRUECOLOR)) 
    {
        result = loadRaw(texture);
    }

    // This is not a mistake! We should not flip image, otherwise texture 
    // will be flipped
    m_flip_x = (header.imageDescriptor & 16) != 0;
    m_flip_y = (header.imageDescriptor & 32) == 0;
    if ((m_flip_x || m_flip_y) && result)
    {
        this->flip(texture);
    }
    return result;
}

bool sad::imageformats::TGALoader::loadRaw(sad::Texture * texture) const
{
    unsigned int raw_image_size = m_bypp * texture->width() * texture->height();
    sad::uchar * begin = &(texture->Buffer->buffer()[0]);
    unsigned int bypp = m_bypp;
    for (unsigned int i = 0; i < raw_image_size; i += bypp)
    {
        m_file->read(reinterpret_cast<char*>(begin), sizeof(sad::uchar) * bypp);
        if (m_file->fail() || m_file->eof())
        {
            return false;
        }
        std::swap(*begin, *(begin + 2));
        begin += 4; 
    }
    return true;
}

bool sad::imageformats::TGALoader::loadCompressed(sad::Texture * texture) const
{
    bool result = true;             // Result
    
    // Allocate buffer and read data
    const unsigned int header_buffer_size = 18;
    std::vector<unsigned char> buffer;
    m_file->seekg(0, std::ios_base::end);
    unsigned int size = static_cast<unsigned int>(m_file->tellg()) - header_buffer_size * sizeof(sad::uchar);
    m_file->seekg(header_buffer_size * sizeof(sad::uchar), std::ios_base::beg);
    buffer.resize(static_cast<size_t>(size), 255);
#ifdef TEXTURE_LOADER_TEST
    printf("RLE buffer size is %d, my bytes per pixel is %d, image's is %d\n", size, m_bypp, texture->Bpp / 8);
#endif  
    m_file->read(reinterpret_cast<char*>(&buffer[0]), size);
    if (m_file->fail() || m_file->eof())
    {
#ifdef TEXTURE_LOADER_TEST
        printf("Reading %d bytes of compressed TGA failed, exiting\n",  size);
#endif          
        buffer.clear();
        return false;
    }
    
    unsigned char * rle_buffer = &(buffer[0]);
    unsigned char * image_buffer = texture->Buffer->buffer();
    unsigned int rle_buffer_pos = 0;
    unsigned int rle_buffer_size = buffer.size();
    unsigned int image_size = texture->width() * texture->height() * (texture->Bpp / 8);
    unsigned int image_pos = 0;
    // A copied pixel
    unsigned char pixel[4];
    pixel[3] = 255;
    
    while (image_pos < image_size && rle_buffer_pos < rle_buffer_size)
    {
        unsigned char chunkheader = rle_buffer[rle_buffer_pos];
        // If raw section
        if (chunkheader < 128)
        {
            chunkheader++;
            // Move to next byte
            rle_buffer_pos++;
#ifdef TEXTURE_LOADER_TEST
            printf("Raw section with length %d at %d. Image position: %d\n", chunkheader, rle_buffer_pos, image_pos);
#endif
            if (rle_buffer_pos + m_bypp - 1 >= rle_buffer_size)
            {
#ifdef TEXTURE_LOADER_TEST
                printf("Buffer position at %d is last, exiting at raw section\n", rle_buffer_pos);
#endif
                return false;
            }

            for(unsigned int i = 0; i < chunkheader; i++)
            {
                if (image_pos >= image_size)
                {
#ifdef TEXTURE_LOADER_TEST
                    printf("Current image position is %d, exiting at raw section\n", image_pos);
#endif                  
                    return false;
                }
                memcpy(image_buffer + image_pos, rle_buffer + rle_buffer_pos, m_bypp);
                // Swap BGR to RGB
                std::swap(*(image_buffer + image_pos), *(image_buffer + image_pos + 2)); 
                rle_buffer_pos += m_bypp;
                image_pos += 4;
            }
        }
        else
        {
            // Unset ID bit
            chunkheader -= 127;
            // Move to next byte
            rle_buffer_pos++;
#ifdef TEXTURE_LOADER_TEST
            printf("Compressed section with %d  repeated byte at %d. Image position: %d\n", chunkheader, rle_buffer_pos, image_pos);
#endif

            if (rle_buffer_pos + m_bypp - 1 >= rle_buffer_size)
            {
#ifdef TEXTURE_LOADER_TEST
                printf("Buffer position at %d is last, exiting at compressed section\n", rle_buffer_pos);
#endif              
                return false;
            }

            memcpy(pixel, rle_buffer + rle_buffer_pos, m_bypp);
            rle_buffer_pos += m_bypp;
            std::swap(*(pixel), *(pixel + 2));
            for(unsigned int i = 0; i < chunkheader; i++)
            {
                if (image_pos >= image_size)
                {
#ifdef TEXTURE_LOADER_TEST
                    printf("Current image position is %d, exiting at compressed section\n", image_pos);
#endif                      
                    return false;
                }
                memcpy(image_buffer + image_pos, pixel, 4);
                image_pos += 4;
            }
        }
    }
    return result;
}

void sad::imageformats::TGALoader::flip(sad::Texture * texture) const
{
    unsigned int height = texture->height();
    unsigned int rowsize = texture->width() * 4; // 4 is four bytes per row
    unsigned int halfpixelwidth = texture->width() / 2;
    unsigned int halfwidth =  halfpixelwidth * 4;
    unsigned int halfheight = height / 2;

    sad::uchar * flipbuffer = new sad::uchar[rowsize];
    sad::uchar * begin = texture->Buffer->buffer();
    if (m_flip_x && (height % 2) == 1)
    {
        unsigned int offset = halfheight * rowsize;
        sad::uchar * p1 = begin + offset;
        sad::uchar * p2 = begin + (offset + rowsize - halfwidth);
        // Flip horizontally
        memcpy(flipbuffer, p1, halfwidth * sizeof(sad::uchar));
        memcpy(p1, p2, halfwidth * sizeof(sad::uchar));
        memcpy(p2, flipbuffer, halfwidth * sizeof(sad::uchar));
    }

    for(unsigned int row = 0; row < halfheight; row++)
    {
        unsigned int offset1 = row * rowsize;
        unsigned int offset2 =(height - 1 - row) * rowsize;
        sad::uchar * begin1 = begin + offset1;
        sad::uchar * begin2 = begin + offset2;
        if (m_flip_x)
        {
            // Flip horizontally first row
            sad::uchar * p1 = begin1;
            sad::uchar * p2 = begin1 + (rowsize - halfwidth);
            memcpy(flipbuffer, p1, halfwidth * sizeof(sad::uchar));
            memcpy(p1, p2, halfwidth * sizeof(sad::uchar));
            memcpy(p2, flipbuffer, halfwidth * sizeof(sad::uchar));

            // Flip horizontally second row         
            p1 = begin2;
            p2 = begin2 + (rowsize - halfwidth);
            memcpy(flipbuffer, p1, halfwidth * sizeof(sad::uchar));
            memcpy(p1, p2, halfwidth * sizeof(sad::uchar));
            memcpy(p2, flipbuffer, halfwidth * sizeof(sad::uchar));
        }
        if (m_flip_y)
        {
            memcpy(flipbuffer, begin1, rowsize * sizeof(sad::uchar));
            memcpy(begin1, begin2, rowsize * sizeof(sad::uchar));
            memcpy(begin2, flipbuffer, rowsize * sizeof(sad::uchar));
        }
    }
    delete[] flipbuffer;
}

#include "imageformats/srgbaloader.h"
#include <cstring>

const int headersignaturesize = 5;

const sad::uchar headersignature[headersignaturesize] = {'S', 'R', 'G', 'B', 'A'};

const int headersize = headersignaturesize + 1;

const int maxlogtexturesize = 14; 

bool sad::imageformats::SRGBALoader::load(FILE * file, sad::Texture * texture)
{
    // Exit on invalid input data
    if (!file || !texture)
    {
        return false;
    }

    sad::uchar header[headersize];
    
    // Exit if unable to read header
    if (fread(header, headersize, 1, file) != 1)
    {
        return false;
    }

    // Exit if signature is invalid
    if (memcmp(header, headersignature, headersignaturesize) != 0)
    {
        return false;
    }

    // Exit if image size is too large
    sad::uchar logtexsize = header[headersize - 1];
    if (logtexsize > maxlogtexturesize)
    {
        return false;
    }

    unsigned int texsize = 1 << static_cast<unsigned int>(logtexsize);
    unsigned int buffersize = texsize * texsize * 4;

    texture->width() = texsize;
    texture->height() = texsize;
    texture->bpp() = 32;
    texture->vdata().resize(buffersize);

    sad::uchar* buffer = &(texture->vdata()[0]);
    if (fread(buffer, buffersize, 1, file) != 1)
    {
        return false;
    }
    return true;
}

sad::imageformats::SRGBALoader::~SRGBALoader()
{
    
}

#include "texture.h"
#include "nextpoweroftwo.h"


sad::freetype::Texture::Texture()
: Width(0), Height(0), IsOnGPU(false), Id(0)
{

}

sad::freetype::Texture::~Texture()
{

}

void sad::freetype::Texture::unload()
{
    if (IsOnGPU)
    {
        glDeleteTextures(1, &Id);
    }
}

void sad::freetype::Texture::copyPixel(int dx, int dy, int sx, int sy, const sad::freetype::Texture* tex)
{
    int dwidth = static_cast<int>(Width);
    int swidth = static_cast<int>(tex->Width);
    Pixels[2 * (dx + dy * dwidth)] = tex->Pixels[2 * (sx + sy * swidth)];
    Pixels[2 * (dx + dy * dwidth) + 1] = tex->Pixels[2 * (sx + sy * swidth) + 1];
}

void sad::freetype::Texture::copyRow(int dx, int dy, int sx, int sy, int width, const sad::freetype::Texture* tex)
{
    for (int i = 0; i < width; i++)
    {
        copyPixel(dx + i, dy, sx + i, sy, tex);
    }
}

void sad::freetype::Texture::copySubImage(int dx, int dy, int sx, int sy, int width, int height, const sad::freetype::Texture* tex)
{
    for (int j = 0; j < height; j++)
    {
        copyRow(dx, dy + j, sx, sy + j, width, tex);
    }
}

void sad::freetype::Texture::storeBitmap(FT_Bitmap & bitmap)
{
    // storeBitmap is not re-entrant
    if (IsOnGPU)
        return;

    // Fallback to empty texture in  case when bitmap is empty
    if (bitmap.width == 0 && bitmap.rows == 0)
    {
        Pixels.resize(8);
        Width = 2.0f;
        Height = 2.0f;
        std::fill_n(Pixels.begin(), 8, 0);
        return;
    }

    unsigned int w = sad::freetype::next_power_of_two( bitmap.width );
    unsigned int h = sad::freetype::next_power_of_two( bitmap.rows );
    Width = (float)w;
    Height = (float)h;
    Pixels.resize(2 * w * h, 0);

    //Fill the data
    for(unsigned int j = 0; j < h; j++) 
    {
        for(unsigned int i = 0; i < w; i++)
        {
            Pixels[2*(i+j*w)]= Pixels[2*(i+j*w)+1] = 
                ((int)i>=bitmap.width || (int)j>=bitmap.rows) ?
                0 : bitmap.buffer[i + bitmap.width*j];
        }
    }
}


void sad::freetype::Texture::upload()
{
    // upload
    if (IsOnGPU)
        return;

    IsOnGPU = true;
    
    glGenTextures(1, &Id );
    GLenum lasterror = glGetError();

    glBindTexture(GL_TEXTURE_2D, Id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexImage2D( GL_TEXTURE_2D, 
                  0, 
                  GL_RGBA2, 
                  static_cast<unsigned int>(Width), 
                  static_cast<unsigned int>(Height), 
                  0, 
                  GL_LUMINANCE_ALPHA, 
                  GL_UNSIGNED_BYTE, 
                  &(Pixels[0]) 
                );	
    lasterror = glGetError(); //-V519
    Pixels.clear();
}

void sad::freetype::Texture::bind()
{
    glBindTexture( GL_TEXTURE_2D, Id);
}

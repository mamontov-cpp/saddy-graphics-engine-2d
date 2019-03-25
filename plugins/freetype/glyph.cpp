#include "glyph.h"
#include "towidechar.h"

#include <imageformats/bmploader.h>

#include "3rdparty/format/format.h"
#include <sadstring.h>


const float sad::freetype::Glyph::tan_20_degrees = 0.36397023426620234f;

sad::freetype::Glyph::Glyph()
: Index(0), Width(0), 
Height(0), TexCoordinateWidth(0), 
TexCoordinateHeight(0), BearingY(0),
Descender(0), AdvanceX(0), 
YMax(0), YMin(0)
{

}

sad::freetype::Glyph::Glyph(FT_Face face, unsigned char c)
{
    sad::Maybe<FT_Glyph> result = sad::freetype::Glyph::glyph(face, c, Index);
    if (result.exists())
    {
        makeGlyph(face, result.value());
    }
    else
    {
        makeEmptyGlyph();
    }
}

void sad::freetype::Glyph::render(float x, float y, float topoffset)
{
    Texture.bind();

    glBegin(GL_QUADS);

    glTexCoord2d(0.0f,0.0f); 
    glVertex2d(x + topoffset, y + BearingY);
    
    glTexCoord2d(0.0f, TexCoordinateHeight); 
    glVertex2d(x, y + Descender);
    
    glTexCoord2d(TexCoordinateWidth, TexCoordinateHeight); 
    glVertex2d(x + Width, y + Descender);

    glTexCoord2d(TexCoordinateWidth, 0.0f); 
    glVertex2d(x + Width + topoffset, y + BearingY);
    
    glEnd();
}


sad::Maybe<FT_Glyph> sad::freetype::Glyph::glyph(FT_Face face, unsigned char c, unsigned int & index)
{
    wchar_t widecharacter = sad::freetype::to_wide_char(c);
    index = FT_Get_Char_Index( face, widecharacter );
    
    sad::Maybe<FT_Glyph> result;

    if (!FT_Load_Glyph( face, index, FT_LOAD_DEFAULT))
    {
        FT_Glyph glyph = NULL;
        if (!FT_Get_Glyph(face->glyph, &glyph))
        {
            result.setValue(glyph);
        }
    }
    return result;
}

sad::String sad::freetype::Glyph::dumpParametes() const
{
    return str(fmt::Format(
        "Width: {0} "
        "Height: {1} "
        "Descender: {2} "
        "BearingY: {3} "
        "AdvanceX: {4}\n") 
        << Width << Height
        << Descender << BearingY
        << AdvanceX
    );
}

void sad::freetype::Glyph::dumpToBMP() const
{
    const std::vector<unsigned char>& source_pixels=  Texture.Pixels;
    std::vector<unsigned char> dest_pixels;
    for(size_t y = 0; y < Texture.Height; y++)  // NOLINT(modernize-loop-convert)
    { 
        // NOLINT(modernize-loop-convert)
        for (size_t x = 0; x < Texture.Width; x++) // NOLINT(modernize-loop-convert)
        {
            unsigned int i = 2 * (x + y * static_cast<unsigned int>(Texture.Width));
//            unsigned char p0 = ((source_pixels[i] & 2) >> 1) * 170 + (source_pixels[i] & 1) * 85;
//            unsigned char p1 = ((source_pixels[i] & 8) >> 3) * 170 + ((source_pixels[i] & 4) >> 2) * 85;
//            unsigned char p2 = ((source_pixels[i] & 32) >> 5) * 170 + ((source_pixels[i] & 16) >> 4) * 85;
//            unsigned char p3 = ((source_pixels[i] & 128) >> 7) * 170 + ((source_pixels[i] & 6) >> 6) * 85;

            unsigned char red = source_pixels[i];
            unsigned char green = source_pixels[i];
            unsigned char blue = source_pixels[i];

            dest_pixels.push_back(red);
            dest_pixels.push_back(green);
            dest_pixels.push_back(blue);
        }
    }

    sad::String data = sad::String::number(Index) + ".bmp";
    sad::imageformats::dumpToBMP(data, static_cast<unsigned int>(Texture.Width), static_cast<unsigned int>(Texture.Height), 3, &(dest_pixels[0]));
}

void sad::freetype::Glyph::makeGlyph(FT_Face face, FT_Glyph glyph)
{
    FT_Glyph_To_Bitmap( &glyph, FT_RENDER_MODE_NORMAL, 0, 1 );
    FT_BitmapGlyph bitmap_glyph = reinterpret_cast<FT_BitmapGlyph>(glyph);
    FT_Bitmap & bitmap = bitmap_glyph->bitmap;

    Texture.storeBitmap(bitmap);

    Width = static_cast<double>(bitmap.width);
    Height = static_cast<double>(bitmap.rows);
    TexCoordinateWidth = Width / Texture.Width;
    TexCoordinateHeight = Height / Texture.Height;

    double diff = static_cast<double>(static_cast<long>(bitmap_glyph->top) - static_cast<long>(bitmap.rows));
    Descender = diff;
    BearingY = Height + Descender;
    AdvanceX = static_cast<double>(face->glyph->advance.x >> 6);
    YMax = face->bbox.yMax;
    YMin = face->bbox.yMin;

    FT_Done_Glyph(glyph);
}

void sad::freetype::Glyph::makeEmptyGlyph()
{
    Width = 0;
    Height = 0;
    Descender = 0;
    BearingY = 0;
    AdvanceX = 0;
    TexCoordinateWidth = 1.0;
    TexCoordinateHeight = 1.0;
    Texture.IsOnGPU = false;
    Texture.Height = 2.0f;
    Texture.Width = 2.0f;
    Texture.Pixels.resize(8);
    std::fill_n(Texture.Pixels.begin(), 8, 0);
}

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
YMax(0), YMin(0), Data(NULL)
{

}

sad::freetype::Glyph::Glyph(FT_Face face, unsigned char c, bool store_texture) : Data(NULL)
{
    sad::Maybe<FT_Glyph> result = sad::freetype::Glyph::glyph(face, c, Index);
    if (result.exists())
    {
        makeGlyph(face, result.value(), store_texture);
    }
    else
    {
        makeEmptyGlyph();
    }
}

void sad::freetype::Glyph::render(float x, float y, float topoffset)
{
    glBegin(GL_TRIANGLES);

    glTexCoord2d(TextureRectangle.p0().x(), TextureRectangle.p0().y());
    glVertex2d(x + topoffset, y + BearingY);
    
    glTexCoord2d(TextureRectangle.p3().x(), TextureRectangle.p3().y());
    glVertex2d(x, y + Descender);

    glTexCoord2d(TextureRectangle.p1().x(), TextureRectangle.p1().y());
    glVertex2d(x + Width + topoffset, y + BearingY);

    glTexCoord2d(TextureRectangle.p3().x(), TextureRectangle.p3().y());
    glVertex2d(x, y + Descender);

    glTexCoord2d(TextureRectangle.p1().x(), TextureRectangle.p1().y());
    glVertex2d(x + Width + topoffset, y + BearingY);

    glTexCoord2d(TextureRectangle.p2().x(), TextureRectangle.p2().y());
    glVertex2d(x + Width, y + Descender);

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

void sad::freetype::Glyph::freeInnerGlyph()
{
    if (Data)
    {
        FT_Done_Glyph(reinterpret_cast<FT_Glyph>(Data));
    }
}

void sad::freetype::Glyph::makeGlyph(FT_Face face, FT_Glyph glyph, bool store_texture)
{
    FT_Glyph_To_Bitmap( &glyph, FT_RENDER_MODE_NORMAL, 0, 1 );
    FT_BitmapGlyph bitmap_glyph = reinterpret_cast<FT_BitmapGlyph>(glyph);
    FT_Bitmap & bitmap = bitmap_glyph->bitmap;

    if (store_texture)
    {
        Texture.storeBitmap(bitmap);

        Width = static_cast<double>(bitmap.width);
        Height = static_cast<double>(bitmap.rows);
        TexCoordinateWidth = Width / Texture.Width;
        TexCoordinateHeight = Height / Texture.Height;
    }
    else
    {
        Data = glyph;
        Width = static_cast<double>(bitmap.width);
        Height = static_cast<double>(bitmap.rows);
    }

    double diff = static_cast<double>(static_cast<long>(bitmap_glyph->top) - static_cast<long>(bitmap.rows));
    Descender = diff;
    BearingY = Height + Descender;
    AdvanceX = static_cast<double>(face->glyph->advance.x >> 6);
    YMax = face->bbox.yMax;
    YMin = face->bbox.yMin;

    if (store_texture)
    {
        FT_Done_Glyph(glyph);
    }
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

#include "glyph.h"
#include "towidechar.h"

#include "3rdparty/format/format.h"

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

void sad::freetype::Glyph::render(float x, float y)
{
	Texture.bind();

	glBegin(GL_QUADS);

	glTexCoord2f(0.0f,0.0f); 
	glVertex2f(x, y + BearingY);
	
	glTexCoord2f(0.0f, TexCoordinateHeight); 
	glVertex2f(x, y + Descender);
	
	glTexCoord2f(TexCoordinateWidth, TexCoordinateHeight); 
	glVertex2f(x + Width, y + Descender);

	glTexCoord2f(TexCoordinateWidth, 0.0f); 
	glVertex2f(x + Width, y + BearingY);
	
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

void sad::freetype::Glyph::makeGlyph(FT_Face face, FT_Glyph glyph)
{
	FT_Glyph_To_Bitmap( &glyph, FT_RENDER_MODE_NORMAL, 0, 1 );
	FT_BitmapGlyph bitmap_glyph = reinterpret_cast<FT_BitmapGlyph>(glyph);
	FT_Bitmap & bitmap = bitmap_glyph->bitmap;

	Texture.storeBitmap(bitmap);

	Width = static_cast<float>(bitmap.width);
	Height = static_cast<float>(bitmap.rows);
	TexCoordinateWidth = Width / Texture.Width;
	TexCoordinateHeight = Height / Texture.Height;

	float diff = static_cast<float>(static_cast<long>(bitmap_glyph->top) - static_cast<long>(bitmap.rows));
	Descender = diff;
	BearingY = Height + Descender;
	AdvanceX = static_cast<float>(face->glyph->advance.x >> 6);
	YMax = face->bbox.yMax;
	YMin = face->bbox.yMin;
}

void sad::freetype::Glyph::makeEmptyGlyph()
{
	Width = 0;
	Height = 0;
	Descender = 0;
	BearingY = 0;
	AdvanceX = 0;
	TexCoordinateWidth = 1.0f;
	TexCoordinateHeight = 1.0f;
	Texture.IsOnGPU = false;
	Texture.Height = 2.0f;
	Texture.Width = 2.0f;
	Texture.Pixels.resize(4);
	std::fill_n(Texture.Pixels.begin(), 4, 0);
}

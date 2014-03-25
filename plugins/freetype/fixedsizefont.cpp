#include "fixedsizefont.h"
#include "towidechar.h"

#include <sadmutex.h>

#include <math.h>

#ifdef _MSC_VER
	#pragma warning(disable: 4996)
#endif

sad::freetype::FixedSizeFont::FixedSizeFont(
	FT_Library library, 
	FT_Face face, 
	unsigned int height
)
: m_on_gpu(false)
{
	requestSize(library, face, height);

	int ppem = face->size->metrics.y_ppem;
	double linespacinginpt = face->bbox.yMax - face->bbox.yMin;
	m_builtin_linespacing = ppem * ((float)(linespacinginpt) / face->units_per_EM);
	m_bearing_y = ppem * ((float)(face->bbox.yMax) / face->units_per_EM);	
	
	for(unsigned int i = 0; i < 256; i++)
	{
		m_glyphs[i] = new sad::freetype::Glyph(face, (unsigned char)i);
	}

	computeKerning(face);
}


sad::freetype::FixedSizeFont::~FixedSizeFont()
{
	for(unsigned int i = 0; i < 256; i++)
	{
		delete m_glyphs[i];
	}
}

void sad::freetype::FixedSizeFont::uploadedTextures(sad::Vector<unsigned int> & textures)
{
	for(unsigned int i = 0; i < 256; i++)
	{
		if (m_glyphs[i]->Texture.IsOnGPU)
		{
			textures << m_glyphs[i]->Texture.Id;
		}
	}
}

void sad::freetype::FixedSizeFont::markTexturesAsUnloaded()
{
	for(unsigned int i = 0; i < 256; i++)
	{
		m_glyphs[i]->Texture.IsOnGPU = false;
	}
}

static sad::Mutex sad_freetype_font_lock;

void sad::freetype::FixedSizeFont::render(
	const sad::String & s, 
	const sad::Point2D & p, 
	float ratio
)
{
	sad_freetype_font_lock.lock();

	if (!m_on_gpu)
	{
		for(unsigned int i = 0; i < 256; i++)
		{
			m_glyphs[i]->Texture.upload();
		}
		m_on_gpu = true;
	}

	sad::String tmp = s;
	tmp.removeAllOccurences("\r");
	sad::StringList list = tmp.split("\n");

	bool previous = false;
	unsigned char prevchar = 0;
	float xbegin = (float)(p.x());
	float curx = xbegin;
	float cury = (float)(p.y() - m_bearing_y);
	
	for(unsigned int i = 0; i < list.size(); i++)
	{
		for(unsigned int j = 0; j < list[i].size(); j++)
		{
			unsigned char curchar = list[i][j];
			if (previous)
			{
				curx += m_kerning_table[prevchar][curchar];
			}

			sad::freetype::Glyph * g = m_glyphs[curchar];
			g->render(curx, cury);

			curx += g->AdvanceX;
			prevchar = curchar;
			previous = true;
		}
		cury -= m_builtin_linespacing * ratio;
		curx = xbegin;
		previous = false;
	}

	sad_freetype_font_lock.unlock();
}

sad::Size2D sad::freetype::FixedSizeFont::size(
	const sad::String & s, 
	float ratio
)
{
	sad::String tmp = s;
	tmp.removeAllOccurences("\r");
	sad::StringList list = tmp.split("\n");

	bool previous = false;
	unsigned char prevchar = 0;
	float curx = 0;
	float maxx = 0;
	for(unsigned int i = 0; i < list.size(); i++)
	{
		curx = 0;
		for(unsigned int j = 0; j < list[i].size(); j++)
		{
			unsigned char curchar = list[i][j];
			if (previous)
			{
				curx += m_kerning_table[prevchar][curchar];
			}

			sad::freetype::Glyph * g = m_glyphs[curchar];
			curx += g->AdvanceX;
			prevchar = curchar;
			previous = true;
		}
		maxx = std::max(maxx, curx);		
	}

	float maxy = 0 ;
	if (list.size() == 1)
	{
		if (fabs(maxx) > 0.001) 
		{
			maxy = m_builtin_linespacing; 
		}
	} 
	else
	{
		maxy = m_builtin_linespacing 
			+ (list.size() - 1) * m_builtin_linespacing * ratio;
	}

	return sad::Size2D(maxx, maxy);
}

void sad::freetype::FixedSizeFont::requestSize(
	FT_Library library,
	FT_Face face, 
	unsigned int height
)
{
	 double pixelheight = height;
	 double ptheight = pixelheight / 96.0 * 72.0;
	 int  pointheight = ((int)ceil(ptheight)) << 6;

	 FT_Size_RequestRec_ req;
	 req.type = FT_SIZE_REQUEST_TYPE_BBOX;
	 req.horiResolution = 96;
	 req.vertResolution = 96;
	 req.height = pointheight;
	 req.width = pointheight;
	   
	 FT_Request_Size(face, &req);
}

void sad::freetype::FixedSizeFont::computeKerning(FT_Face face)
{
	if (FT_HAS_KERNING(face))
	{
		FT_Vector kerning;
		for(unsigned int i = 0 ; i < 256; i++)
		{
			wchar_t c1 = sad::freetype::to_wide_char((unsigned int)i);
			unsigned int index1 = FT_Get_Char_Index(face, c1);
			for(unsigned int j = 0; j < 256; j++)
			{
				wchar_t c2 = sad::freetype::to_wide_char((unsigned int)j);
				unsigned int index2 = FT_Get_Char_Index(face, c2);
				FT_Get_Kerning(face, index1, index2, FT_KERNING_DEFAULT, &kerning);
				m_kerning_table[i][j] = (float)(kerning.x >> 6);
			}
		}
	}
	else
	{
		std::fill_n(&(m_kerning_table[0][0]), 256 * 256, 0.0f);
	}
}

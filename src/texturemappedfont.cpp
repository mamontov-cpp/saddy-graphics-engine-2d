#include "texturemappedfont.h"
#include "texturemanager.h"
#include "renderer.h"

sad::TextureMappedFont::TextureMappedFont() : sad::Font(), m_texture(NULL)
{

}

sad::TextureMappedFont::~TextureMappedFont()
{

}

sad::Size2D sad::TextureMappedFont::size(const sad::String & str)
{
	sad::Size2D result;
	// If loading was failed, return (0, 0) as size
	if (m_texture == NULL)
		return result;
	
	// A font can have various sizes, so we need ratio to compute new width
	// of glyph
	unsigned int height = this->m_size;
	double ratio = (double)height / m_typical_glyph.Height;
	unsigned int width = m_typical_glyph.Width * ratio;
	// Lets break string into lines
	sad::StringList lines = str.split("\n");
	// Compute result
	result.Height = height * lines.size();
	for(unsigned int i = 0; i < lines.size(); i++)
	{
		double linewidth = lines[i].size() * width;
		result.Width = std::max(linewidth, result.Width);
	}
	return result;
}


void  sad::TextureMappedFont::render(const sad::String & str,const sad::Point2D & p)
{
	// If loading was failed, do nothing
	if (m_texture == NULL)
		return;

	double x = p.x();
	double y = p.y();


	// A font can have various sizes, so we need ratio to compute new width
	// of glyph
	unsigned int height = this->m_size;
	double ratio = (double)height / m_typical_glyph.Height;
	unsigned int width = m_typical_glyph.Width * ratio;
	
	sad::String string = str;
	string.removeAllOccurences("\r");
	m_texture->enable();

	unsigned int glyphheight = 0;
	unsigned int glyphwidth = 0;
	
	glBegin(GL_QUADS);
	for(unsigned int i = 0;  i < string.length(); i++)
	{
		unsigned char glyphchar = *reinterpret_cast<unsigned char*>(&(string[i]));
		sad::Rect2D & glyph = m_glyphs[ glyphchar ];

		glyphheight = m_sizes[glyphchar].Height;
		glyphwidth = m_sizes[glyphchar].Width * ratio; 

		glTexCoord2f(glyph[0].x(), glyph[0].y()); 
 		glVertex2f(x, y);

		glTexCoord2f(glyph[1].x(), glyph[1].y()); 
 		glVertex2f(x + glyphwidth, y);

		glTexCoord2f(glyph[2].x(), glyph[2].y()); 
 		glVertex2f(x + glyphwidth, y - glyphheight);

		glTexCoord2f(glyph[3].x(), glyph[3].y()); 
 		glVertex2f(x, y - glyphheight);

		if (string[i] != '\n')
		{
			x += width;
		}
		else
		{
			x = p.x();
			y -= height;
		}
	}
	glEnd();
	
}

bool sad::TextureMappedFont::load(const sad::String & filename, sad::Renderer * r)
{
	return this->load(filename + ".png", filename + ".cfg", r);
}

bool sad::TextureMappedFont::load(
	const sad::String & texturefilename,  
	const sad::String & configfilename,
	sad::Renderer * r
)
{
	// If renderer is null, set renderer to global
	if (r == NULL)
		r = sad::Renderer::ref();

	// Trying to load a textures
	m_texture = new sad::Texture;
	r->textures()->add(texturefilename, m_texture);
	if (!m_texture->load(texturefilename, r))
	{
		r->textures()->remove(texturefilename);
		return false;
	}
	
	bool result = true;
	FILE * fl = fopen(configfilename.data(), "rt");
	if (fl)
	{
		// Count is the first line of format file
		int count = 0;

		// If failed to read file,result is false
		fscanf(fl, "%d\n", &count);
		if (ferror(fl)) 
		{
			result = false;
		}

		// Try to load each glyph
		unsigned int x1 = 0 , y1 = 0, x2 = 0, y2 = 0;
		unsigned char c = 0;
		for (int i = 0; i < count && result; i++ )
		{
			fscanf(fl,"%c %u %u %u %u\n", &c, &x1, &y1, &x2,&y2);
			if (ferror(fl))
			{
				result = false;
			}

			m_sizes[c].Width = abs((int)(x2 - x1));
			m_sizes[c].Height = abs((int)(y2 - y1));
			
			sad::Rect2D rect(
				sad::Point2D(
					(double)x1 / m_texture->width(), 
					(double)y1 / m_texture->height()
				),
				sad::Point2D(
					(double)x2 / m_texture->width(), 
					(double)y2 / m_texture->height()				
				)
			);
			m_glyphs[c] = rect; 
		}
		fclose(fl);
	} 
	else
	{
		result = false;
	}

	// If failed to load, remove texture
	if (result == false)
	{	
		r->textures()->remove(texturefilename);
	} 
	else
	{
		// We take 'A' as a basic glyph
		m_typical_glyph = m_sizes['A'];
		m_size = m_typical_glyph.Height;
	}
	return result;
}

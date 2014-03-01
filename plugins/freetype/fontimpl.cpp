#include "fontimpl.h"
#include "fixedsizefont.h"

#include <pipeline/pipeline.h>

#include <util/deletetexturetask.h>

#include <renderer.h>

sad::freetype::FontImpl::FontImpl() 
: m_library(0), m_face(0), 
m_cached_size(-1), m_font(NULL),
m_created_now(true)
{
	// If freetype failed, than do nothing
	if (FT_Init_FreeType(&m_library))  
	{
		m_library = 0;
	}
}

sad::freetype::FontImpl::~FontImpl()
{
	cleanup();
	if (m_library != 0)
		FT_Done_FreeType(m_library);
}




bool sad::freetype::FontImpl::load(const sad::String & filename)
{
	cleanup();
	if (m_library == 0)
		return false;
	// Exit if face creation failed
	if (FT_New_Face( m_library, filename.data(), 0, &m_face )) 
	{
		m_face = 0;
		return false;
	}
	return true;
}

void sad::freetype::FontImpl::render(
		const sad::String & str,
		const sad::Point2D & p,
		float ratio
)
{
	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);	
    glMatrixMode(GL_MODELVIEW);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

	this->fontForSize(m_cached_size)->render(str, p, ratio);
	
	glPopAttrib();
}

void sad::freetype::FontImpl::unload(sad::Renderer * r)
{
	sad::Vector<unsigned int> textures;
	for(SizeMap::iterator it = m_size_cache.begin(); it != m_size_cache.end(); it++)
	{
		it.value()->uploadedTextures(textures);
		it.value()->markTexturesAsUnloaded();
	}
	if (!r)
	{
		glDeleteTextures(textures.size(), &(textures[0]));
	}
	else
	{
		if (r->isOwnThread())
		{
			glDeleteTextures(textures.size(), &(textures[0]));
		}
		else
		{
			if (r->running())
			{
				r->pipeline()->append(new  sad::util::DeleteTextureTask(textures));
			}
		}
	}
}

void sad::freetype::FontImpl::setSize(unsigned int size)
{
	bool changed = size != m_cached_size;
	m_cached_size = size;
	if (changed)
	{
		m_font = NULL;
		this->fontForSize(size);
	}
}

sad::Size2D sad::freetype::FontImpl::size(const sad::String & str, float ratio)
{
	return this->fontForSize(m_cached_size)->size(str, ratio);
}

float sad::freetype::FontImpl::builtinLineSpacing() const
{
	return this->fontForSize(m_cached_size)->builtinLineSpacing();
}

sad::freetype::FixedSizeFont * sad::freetype::FontImpl::fontForSize(
	unsigned int size
) const
{
	sad::freetype::FontImpl * me = const_cast<sad::freetype::FontImpl *>(this);
	
	if (m_font != NULL && size == m_cached_size)
	{
		return m_font;
	}

	if (m_size_cache.contains(size))
	{
		me->m_cached_size = size;
		me->m_font = m_size_cache[size];
		return m_font;
	}

	sad::freetype::FixedSizeFont * f = new sad::freetype::FixedSizeFont(
		m_library,
		m_face, 
		size
	);
	me->m_cached_size = size;
	me->m_font = f;
	me->m_size_cache.insert(size, f);

	return f;
}

void sad::freetype::FontImpl::cleanup()
{
	if (m_face != 0)
	{
		FT_Done_Face(m_face);
	}
	for(SizeMap::iterator it = m_size_cache.begin(); it != m_size_cache.end(); it++)
	{
		delete it.value();
	}
	m_size_cache.clear();
	m_font = NULL;
	m_created_now = true;
}

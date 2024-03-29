#include "fontimpl.h"
#include "fixedsizefont.h"

#include <pipeline/pipeline.h>

#include <util/deletetexturetask.h>

#include <renderer.h>

#include <sadscopedlock.h>

sad::freetype::FontImpl::FontImpl() 
: m_library(0), m_face(0), 
m_cached_size(-1), m_font(nullptr),
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


static unsigned int ___minimumCacheFontSize = 10;
static unsigned int ___maximumCacheFontSize = 36;

void sad::freetype::FontImpl::setMinCacheFontSize(unsigned int size)
{
    ___minimumCacheFontSize = size;
}

void sad::freetype::FontImpl::setMaxCacheFontSize(unsigned int size)
{
    ___maximumCacheFontSize = size;
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
    // Build partially a cache for freetype
    for(unsigned int i = ___minimumCacheFontSize; i < ___maximumCacheFontSize; ++i)
    {
        (void)this->fontForSize(i);
    }
    return true;
}

void sad::freetype::FontImpl::render(
        const sad::String & str,
        const sad::Point2D & p,
        float ratio,
        int flags
)
{
    glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);	
    glMatrixMode(GL_MODELVIEW);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

    this->fontForSize(m_cached_size)->render(str, p, ratio, flags);
    
    glPopAttrib();
}

void sad::freetype::FontImpl::fillGeometries(const sad::Font::GeometryRenderData& data, sad::os::GLFontGeometries& g, const sad::String & str, const sad::Point2D & p, int flags, float ratio)
{
    this->fontForSize(m_cached_size)->fillGeometries(data, g, str, p, flags, ratio);
}

sad::Texture * sad::freetype::FontImpl::renderToTexture(
    const sad::String & string,
    unsigned int height 
)
{
    return this->fontForSize(m_cached_size)->renderToTexture(string, m_library, m_face, height);
}

float sad::freetype::FontImpl::ascent() const
{
    return this->fontForSize(m_cached_size)->ascent();
}

void sad::freetype::FontImpl::unload(sad::Renderer * r)
{
    sad::Vector<unsigned int> textures;
    for(SizeMap::iterator it = m_size_cache.begin(); it != m_size_cache.end(); ++it)
    {
        it.value()->uploadedTextures(textures);
        it.value()->markTexturesAsUnloaded();
    }
    if (!r)
    {
        if (textures.count())
        {
            glDeleteTextures(textures.size(), &(textures[0]));
        }
    }
    else
    {
        if (r->isOwnThread())
        {
            if (textures.count())
            {
                glDeleteTextures(textures.size(), &(textures[0]));
            }
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
        m_font = nullptr;
        this->fontForSize(size);
    }
}

sad::Size2D sad::freetype::FontImpl::size(const sad::String & str, float ratio, int flags)
{
    return this->fontForSize(m_cached_size)->size(str, ratio, flags);
}

float sad::freetype::FontImpl::builtinLineSpacing() const
{
    return this->fontForSize(m_cached_size)->builtinLineSpacing();
}

sad::String sad::freetype::FontImpl::dumpGlyphParameters() const
{
    return this->fontForSize(m_cached_size)->dumpGlyphParameters();
}

sad::freetype::FixedSizeFont * sad::freetype::FontImpl::fontForSize(
    unsigned int size
) const
{
    sad::ScopedLock lock(&(const_cast<sad::Mutex&>(m_mtx)));
    sad::freetype::FontImpl * me = const_cast<sad::freetype::FontImpl *>(this);
    
    if (m_font != nullptr && size == m_cached_size)
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
    for(SizeMap::iterator it = m_size_cache.begin(); it != m_size_cache.end(); ++it)
    {
        delete it.value();
    }
    m_size_cache.clear();
    m_font = nullptr;
    m_created_now = true;
}

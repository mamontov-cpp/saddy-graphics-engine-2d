#include <freetype/font.h>
#include "fontimpl.h"
#include <log/log.h>

#include <renderer.h>

#include <util/fs.h>

#include <resource/resourcefile.h>


DECLARE_SOBJ_INHERITANCE(sad::freetype::Font, sad::Font);

sad::freetype::Font::Font()
: m_dptr(new sad::freetype::FontImpl()), m_renderer(NULL)
{

}

bool sad::freetype::Font::load(
        const sad::resource::ResourceFile & file,
        sad::Renderer * r,
        const picojson::value& /*options*/
)
{
    m_renderer = r;
    bool result = load(file.name());
    if (!result && !util::isAbsolutePath(file.name()))
    {
        sad::String newpath = util::concatPaths(r->executablePath(), file.name());
        result = load(newpath);
    }
    return result;
}

bool sad::freetype::Font::load(const sad::String & filename)
{
    return m_dptr->load(filename);
}

void sad::freetype::Font::render(const sad::String & str,const sad::Point2D & p, sad::Font::RenderFlags flags)
{
    m_dptr->setSize(m_size);

    this->setCurrentColor();
    m_dptr->render(str, p, m_linespacing_ratio, flags);
    this->restoreColor();
}


sad::Texture * sad::freetype::Font::renderToTexture(
    const sad::String & string,
    unsigned int height 
)
{
    unsigned int size = m_size;
    m_dptr->setSize(height);
    sad::Texture *  result = m_dptr->renderToTexture(string, height);
    m_dptr->setSize(size);
    return result;
}

float sad::freetype::Font::builtinLineSpacing() const
{
    return m_dptr->builtinLineSpacing();
}

sad::Size2D sad::freetype::Font::size(const sad::String & str, sad::Font::RenderFlags flags)
{
    m_dptr->setSize(m_size);
    return m_dptr->size(str, m_linespacing_ratio, flags);
}

float sad::freetype::Font::ascent() const
{
    return m_dptr->ascent();
}

void sad::freetype::Font::unloadFromGPU()
{
    m_dptr->unload(m_renderer);
}

sad::freetype::Font::~Font()
{
    m_dptr->unload(m_renderer);
    delete m_dptr;
}

sad::String sad::freetype::Font::dumpGlyphParameters() const
{
    return m_dptr->dumpGlyphParameters();
}

// Uncommend to enable glyph rendering debug
// #define FREETYPE_GLYPH_DEBUG

#ifdef FREETYPE_GLYPH_DEBUG

static unsigned const int render_rect_indices_count = 4;

static unsigned int render_rect_indices[render_rect_indices_count][2] = 
{
    {0, 1},
    {1, 2},
    {2, 3},
    {3, 0}
};
/*! Renders rectangle
    \param[in] x1 X coordinate of first point
    \param[in] y1 Y coordinate of first point
    \param[in] x2 X coordinate of second point
    \param[in] y2 Y coordinate of second point
    \param[in] c  ciolor
 */
void debug_render_rect(double x1, double y1, double x2, double y2, const sad::Color & c)
{
    sad::Rect2D r(x1, y1, x2, y2);

    glDisable(GL_TEXTURE_2D);
        
    glBegin(GL_LINES);
    glColor3ub(c.r(), c.g(),  c.b());

    for(int i = 0; i < render_rect_indices_count; i++)
    {
        const sad::Point2D & p1 = r[render_rect_indices[i][0]];
        const sad::Point2D & p2 = r[render_rect_indices[i][1]];

        glVertex2f((float)(p1.x()), (float)(p1.y()));
        glVertex2f((float)(p2.x()), (float)(p2.y()));
    }

    glEnd();

    glEnable(GL_TEXTURE_2D);
}

#endif

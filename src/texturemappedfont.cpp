#include "texturemappedfont.h"
#include "texture.h"
#include "renderer.h"
#include "os/glheaders.h"
#include "os/glfontgeometries.h"
#include "resource/resourcefile.h"

#include "util/fs.h"

// A tangent for 20 degrees angle
#define TAN_20_DEGREES  (0.36397023426620234)

DECLARE_SOBJ_INHERITANCE(sad::TextureMappedFont, sad::Font);

sad::TextureMappedFont::TextureMappedFont()  //-V730
: sad::Font(), 
  m_texture(NULL),
  m_builtin_linespacing(0), 
  m_size_ratio(1.0),
  m_ascent(0)
{

}

sad::TextureMappedFont::~TextureMappedFont()
{
    if (m_texture)
    {
        delete m_texture;
    }
}



sad::Size2D sad::TextureMappedFont::size(const sad::String & str, sad::Font::RenderFlags flags)
{
    sad::Size2D result;
    // If loading was failed, return (0, 0) as size
    if (m_texture == NULL)
        return result;
    
    // A font can have various sizes, so we need ratio to compute new width
    // of glyph
    double height = m_builtin_linespacing * m_size_ratio;

    // Lets break string into lines
    sad::String tmp = str;
    tmp.removeAllOccurences("\r");
    sad::StringList lines = str.split("\n", sad::String::KEEP_EMPTY_PARTS);
    
    double italicoffset = ((double)m_size) * TAN_20_DEGREES;

    // Compute result
    for(unsigned int i = 0; i < lines.size(); i++)
    {
        double linewidth = 0;
        sad::String & string = lines[i];
        for(unsigned int j = 0; j < string.length(); j++)
        {
            unsigned char c = *reinterpret_cast<unsigned char*>(&(string[j]));
            linewidth += m_leftbearings[c] * m_size_ratio;
            linewidth += m_sizes[c].Width * m_size_ratio;
            if ((flags & sad::Font::FRF_Bold) != 0)
            {
                linewidth += 2;
            }
            // It might not be the best idea to add negative right bearing at end of string
            // since some fonts would not guarantee it to be correct, so perform this more carefully
            if ((m_rightbearings[c] >= 0) || (j != string.length() - 1))
            {
                linewidth += m_rightbearings[c] * m_size_ratio;
            }
        }
        if ((flags & sad::Font::FRF_Italic) != 0)
        {
            linewidth += italicoffset;
        }
        result.Width = std::max(linewidth, result.Width);
    }

    if (lines.size() <= 1)
    {
        result.Height = (lines.size() == 1) ? height : 0;
    }
    else
    {
        result.Height = height + height * (lines.size() - 1) * m_linespacing_ratio; 
    }
    return result;
}


void  sad::TextureMappedFont::render(const sad::String & str,const sad::Point2D & p, sad::Font::RenderFlags flags)
{
    // If loading was failed, do nothing
    if (m_texture == NULL)
        return;

    double x = p.x();
    double y = p.y();

    
    sad::String string = str;
    string.removeAllOccurences("\r");
#ifdef LOG_RENDERING
    SL_LOCAL_INTERNAL(
        fmt::Format("Before binding texture {0}")
        << glGetError(), 
        *sad::Renderer::ref()
    );	
#endif
    m_texture->bind();
#ifdef LOG_RENDERING
    SL_LOCAL_INTERNAL(
        fmt::Format("After binding texture {0}")
        << glGetError(), 
        *sad::Renderer::ref()
    );
    SL_LOCAL_INTERNAL(
        fmt::Format("Before attr {0}")
        << glGetError(), 
        *sad::Renderer::ref()
    );
#endif
    glPushAttrib( GL_LIGHTING_BIT | GL_ENABLE_BIT);
#ifdef LOG_RENDERING
    GLint attribstackdepth = 0;
    glGetIntegerv(GL_ATTRIB_STACK_DEPTH, &attribstackdepth);
    SL_LOCAL_INTERNAL(
        fmt::Format("After glPushAttrib {0} stack depth is {1}")
        << glGetError()
        << attribstackdepth, 		
        *sad::Renderer::ref()
    );
#endif
    glDisable(GL_LIGHTING);
#ifdef LOG_RENDERING
    SL_LOCAL_INTERNAL(
        fmt::Format("After glDisable(GL_LIGHTING) {0}")
        << glGetError(), 
        *sad::Renderer::ref()
    );
#endif
    glEnable(GL_COLOR_MATERIAL);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#ifdef LOG_RENDERING
    SL_LOCAL_INTERNAL(
        fmt::Format("After attr {0}")
        << glGetError(), 
        *sad::Renderer::ref()
    );
    SL_LOCAL_INTERNAL(
        fmt::Format("Before current color {0}")
        << glGetError(), 
        *sad::Renderer::ref()
    );
#endif
    this->setCurrentColor();
#ifdef LOG_RENDERING
    glGetIntegerv(GL_ATTRIB_STACK_DEPTH, &attribstackdepth);
    SL_LOCAL_INTERNAL(
        fmt::Format("After current color {0} {1}")
        << glGetError()
        << attribstackdepth, 
        *sad::Renderer::ref()
    );
#endif
    unsigned int glyphheight;
    unsigned int glyphwidth = 0;
#ifdef LOG_RENDERING
    SL_LOCAL_INTERNAL(
        fmt::Format("Before quads {0}")
        << glGetError(), 
        *sad::Renderer::ref()
    );
#endif
    glBegin(GL_TRIANGLES);

    float italicoffset = ((float)m_size) * TAN_20_DEGREES;
    for(unsigned int i = 0;  i < string.length(); i++)
    {
        unsigned char glyphchar = *reinterpret_cast<unsigned char*>(&(string[i]));
        sad::Rect2D & glyph = m_glyphs[ glyphchar ];

        if (string[i] != '\n' && string[i] != '\r')
        {
            x += m_leftbearings[ string[i] ] * m_size_ratio;
            glyphheight = (unsigned int)(m_sizes[glyphchar].Height * m_size_ratio);
            glyphwidth =  (unsigned int)(m_sizes[glyphchar].Width * m_size_ratio);

            int count = 1;
            if ((flags & sad::Font::FRF_Bold) != 0) 
            {
                count = 3;
            }
            float topoffset = 0;
            if ((flags & sad::Font::FRF_Italic) != 0)
            {
                topoffset = italicoffset;
            }

            for (size_t i = 0; i < count; i++)
            {
                glTexCoord2f((GLfloat)glyph[0].x(), (GLfloat)glyph[0].y());
                glVertex2f((GLfloat)(x + i  + topoffset), (GLfloat)y);

                glTexCoord2f((GLfloat)(glyph[3].x()), (GLfloat)(glyph[3].y()));
                glVertex2f((GLfloat)(x + i), (GLfloat)(y - glyphheight));

                glTexCoord2f((GLfloat)glyph[1].x(), (GLfloat)glyph[1].y());
                glVertex2f((GLfloat)(x + glyphwidth + i + topoffset), (GLfloat)y);

                glTexCoord2f((GLfloat)(glyph[3].x()), (GLfloat)(glyph[3].y()));
                glVertex2f((GLfloat)(x + i), (GLfloat)(y - glyphheight));

                glTexCoord2f((GLfloat)glyph[1].x(), (GLfloat)glyph[1].y());
                glVertex2f((GLfloat)(x + glyphwidth + i + topoffset), (GLfloat)y);

                glTexCoord2f((GLfloat)glyph[2].x(), (GLfloat)glyph[2].y());
                glVertex2f((GLfloat)(x + glyphwidth + i), (GLfloat)(y - glyphheight));
            }
        }
        if (string[i] != '\n')
        {
            x += glyphwidth;
            if ((flags & sad::Font::FRF_Bold) != 0)
            {
                x += 2;
            }
            x += m_rightbearings[ string[i] ] * m_size_ratio;
        }
        else
        {
            x = p.x();
            y -= m_builtin_linespacing * m_size_ratio * m_linespacing_ratio;
        }
    }
    glEnd();
#ifdef LOG_RENDERING
    SL_LOCAL_INTERNAL(
        fmt::Format("After quads {0}")
        << glGetError(), 
        *sad::Renderer::ref()
    );
    SL_LOCAL_INTERNAL(
        fmt::Format("Restoring color {0}")
        << glGetError(), 
        *sad::Renderer::ref()
    );
#endif
    this->restoreColor();
#ifdef LOG_RENDERING
    glGetIntegerv(GL_ATTRIB_STACK_DEPTH, &attribstackdepth);
    SL_LOCAL_INTERNAL(
        fmt::Format("Restored color {0} stack depth {1}")
        << glGetError()
        << attribstackdepth, 
        *sad::Renderer::ref()
    );
#endif
    glPopAttrib();
#ifdef LOG_RENDERING
    SL_LOCAL_INTERNAL(
        fmt::Format("Popped attrib {0}")
        << glGetError(), 
        *sad::Renderer::ref()
    );
#endif
}


void sad::TextureMappedFont::fillGeometries(const sad::Font::GeometryRenderData& data, sad::os::GLFontGeometries& g, const sad::String & str, const sad::Point2D & p, sad::Font::RenderFlags flags)
{
    // If loading was failed, do nothing
    if (m_texture == NULL)
        return;

    double x = p.x();
    double y = p.y();

    sad::Vector<double> vertexes, tcs;
    sad::Vector<float> colors;

    sad::String string = str;
    string.removeAllOccurences("\r");

    unsigned int glyphwidth = 0;

    float italicoffset = TAN_20_DEGREES * static_cast<float>(m_size);
    for (unsigned int i = 0; i < string.length(); i++)
    {
        unsigned char glyphchar = *reinterpret_cast<unsigned char*>(&(string[i]));
        sad::Rect2D & glyph = m_glyphs[glyphchar];

        if (string[i] != '\n' && string[i] != '\r')
        {
            x += m_size_ratio * static_cast<float>(m_leftbearings[string[i]]);
            unsigned int glyphheight = static_cast<unsigned int>(m_sizes[glyphchar].Height * m_size_ratio);
            glyphwidth = static_cast<unsigned int>(m_sizes[glyphchar].Width * m_size_ratio);

            size_t count = 1;
            if ((flags & sad::Font::FRF_Bold) != 0)
            {
                count = 3;
            }
            float topoffset = 0;
            if ((flags & sad::Font::FRF_Italic) != 0)
            {
                topoffset = italicoffset;
            }

            for (size_t j = 0; j < count; j++)
            {
                tcs.push_back(glyph[0].x());
                tcs.push_back(glyph[0].y());

                vertexes.push_back(x + j + topoffset);
                vertexes.push_back(y);

                tcs.push_back(glyph[3].x());
                tcs.push_back(glyph[3].y());

                vertexes.push_back(x + j);
                vertexes.push_back(y - glyphheight);

                tcs.push_back(glyph[1].x());
                tcs.push_back(glyph[1].y());

                vertexes.push_back(x + glyphwidth + j + topoffset);
                vertexes.push_back(y);

                tcs.push_back(glyph[3].x());
                tcs.push_back(glyph[3].y());

                vertexes.push_back(x + j);
                vertexes.push_back(y - glyphheight);

                tcs.push_back(glyph[1].x());
                tcs.push_back(glyph[1].y());

                vertexes.push_back(x + glyphwidth + j + topoffset);
                vertexes.push_back(y);

                tcs.push_back(glyph[2].x());
                tcs.push_back(glyph[2].y());

                vertexes.push_back(x + glyphwidth + j);
                vertexes.push_back(y - glyphheight);
            }
        }
        if (string[i] != '\n')
        {
            x += glyphwidth;
            if ((flags & sad::Font::FRF_Bold) != 0)
            {
                x += 2.0;
            }
            x += m_size_ratio * static_cast<float>(m_rightbearings[string[i]]);
        }
        else
        {
            x = p.x();
            y -= m_builtin_linespacing * m_size_ratio * m_linespacing_ratio;
        }
    }

    g.append(data.Renderer, m_texture, vertexes, tcs, data.OwnColor, data.Color);
}

sad::Texture * sad::TextureMappedFont::renderToTexture(const sad::String & str)
{
    if (m_texture == NULL)
        return NULL;

    sad::Texture * result = new sad::Texture();
    
    sad::String rendered_string = str;
    rendered_string.removeAllOccurences("\n");
    rendered_string.removeAllOccurences("\r");

    sad::Vector<sad::Point2I> from;
    sad::Vector<sad::Point2I> to;

    int height = 0;
    int width = 0;
    // Compute size of texture
    for(unsigned int i = 0; i < rendered_string.size(); i++)
    {
        unsigned char glyphchar = *reinterpret_cast<unsigned char*>(&(rendered_string[i]));

        sad::Rect2D & glyph = m_glyphs[ glyphchar ];

        sad::Point2I from_point(
            (int)(glyph[0].x() * m_texture->width()),
            (int)(glyph[0].y() * m_texture->height())
        );
        sad::Point2I to_point(
            (int)(glyph[2].x() * m_texture->width()),
            (int)(glyph[2].y() * m_texture->height())
        );

        from << from_point;
        to << to_point;
        width += to_point.x() - from_point.x();
        height = std::max(height, to_point.y() - from_point.y());
    }

    result->width() = width + 1;
    result->height() = height + 1;
    result->bpp() = 32;
    sad::Vector<sad::uchar>& vdata = reinterpret_cast<sad::Texture::DefaultBuffer*>(result->Buffer)->Data;
    vdata.resize(4 * (width + 1) * (height + 1), 255);
    
    // Fill alpha with zero
    for(size_t i = 0; i < vdata.size(); i+= 4)
    {
        vdata[i + 3] = 0;
    }

    // Paint font
    int x = 0;
    for(unsigned int i = 0; i < rendered_string.size(); i++)
    {
        //unsigned char glyphchar = *reinterpret_cast<unsigned char*>(&(rendered_string[i]));

        //sad::Rect2D & glyph = m_glyphs[ glyphchar ];

        sad::Point2I from_point = from[i];
        sad::Point2I to_point = to[i];

        int y = height - (to_point.y() - from_point.y());
        
        for(int iy = from_point.y(); iy < to_point.y(); iy++)
        {
            for(int ix = from_point.x(); ix < to_point.x(); ix++)
            {
                int py = y + iy - from_point.y();
                int px = x + ix - from_point.x();

                sad::uchar* sourcepix = m_texture->pixel(iy, ix);
                sad::uchar* destpix = result->pixel(py, px);
                if (sourcepix[3] == 0) 
                {
                    destpix[0] = 255;
                    destpix[1] = 255;
                    destpix[2] = 255;
                    destpix[3] = 255;
                }
                else
                {
                    destpix[0] = 255 - sourcepix[0];
                    destpix[1] = 255 - sourcepix[1];
                    destpix[2] = 255 - sourcepix[2];
                    destpix[3] = sourcepix[3];
                }
            }
        }

        x += to_point.x() - from_point.x();
    }
    

    return result;
}


bool sad::TextureMappedFont::load(
        const sad::resource::ResourceFile & file,
        sad::Renderer * r,
        const picojson::value& options
)
{
    bool result = load(file.name(), r);
    if (!result && !util::isAbsolutePath(file.name()))
    {
        sad::String newpath = util::concatPaths(r->executablePath(), file.name());
        result = load(newpath, r);
    }
    return result;
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
    sad::Texture * oldtexture = m_texture;
    m_texture = new sad::Texture;
    m_texture->setRenderer(r);
    if (!m_texture->load(texturefilename, r))
    {
        delete m_texture;
        m_texture = oldtexture;
        return false;
    }
    
    bool result = true;
    FILE * fl = fopen(configfilename.data(), "rt");
    if (fl)
    {
        // Count is the first line of format file
        int count = 0;

        // If failed to read file,result is false
        int integralspacing = 0;
        int integralascent = 0;
        int test = fscanf(fl, "%d %d", &integralspacing, &integralascent);
        m_builtin_linespacing = static_cast<float>(integralspacing);
        m_ascent = static_cast<float>(integralascent);
        if (ferror(fl)) 
        {
            result = false;
        }

        // Try to load each glyph
        unsigned int x1 = 0 , y1 = 0, x2 = 0, y2 = 0;
        unsigned char c = 0;
        for (int i = 0; i < 256 && result; i++ )
        {
            test = fscanf(fl,
                   "%u %u %u %u %d %d\n", 
                   &x1, 
                   &y1, 
                   &x2, 
                   &y2, 
                   m_leftbearings + i, 
                   m_rightbearings + i
                  );
            if (ferror(fl))
            {
                result = false;
            }

            m_sizes[i].Width = abs((int)(x2 - x1));
            m_sizes[i].Height = abs((int)(y2 - y1));
            
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
            m_glyphs[i] = rect; 
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
        delete m_texture;
        m_texture = oldtexture;
    } 
    else
    {
        if (oldtexture)
        {
            delete oldtexture;
        }
        m_size_ratio = (float)(this->sad::Font::size()) / m_builtin_linespacing;
    }
    return result;
}

void sad::TextureMappedFont::unloadFromGPU()
{
    if (m_texture)
    {
        m_texture->unloadFromGPU();
    }
}

float sad::TextureMappedFont::builtinLineSpacing() const
{
    return m_builtin_linespacing * m_size_ratio;
}

float sad::TextureMappedFont::ascent() const
{
    return m_ascent * m_size_ratio;
}

void sad::TextureMappedFont::setSize(unsigned int size)
{
    m_size_ratio = (float)size / m_builtin_linespacing;
}

#include "fixedsizefont.h"
#include "towidechar.h"
#include "packer.h"

#include "os/glfontgeometries.h"

#include <sadmutex.h>

#include <3rdparty/format/format.h>

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
    m_height = static_cast<float>(height);
    int ppem = face->size->metrics.y_ppem;
    double linespacinginpt = face->bbox.yMax - face->bbox.yMin;
    m_builtin_linespacing = ppem * (static_cast<float>(linespacinginpt) / face->units_per_EM);
    m_bearing_y = ppem * (static_cast<float>(face->bbox.yMax) / face->units_per_EM);	
    
    for(unsigned int i = 0; i < 256; i++)
    {
        m_glyphs[i] = new sad::freetype::Glyph(face, static_cast<unsigned char>(i), false);
    }

    m_texture = sad::freetype::Packer::pack(m_glyphs);

    for (unsigned int i = 0; i < 256; i++)
    {
        m_glyphs[i]->freeInnerGlyph();
    }

    computeKerning(face);
}


sad::freetype::FixedSizeFont::~FixedSizeFont()
{
    for(unsigned int i = 0; i < 256; i++)
    {
        delete m_glyphs[i];
    }
    delete m_texture;
}

void sad::freetype::FixedSizeFont::uploadedTextures(sad::Vector<unsigned int> & textures)
{
    if (m_texture->IsOnGPU)
    {
        textures << m_texture->Id;
    }
}

float sad::freetype::FixedSizeFont::ascent() const
{
    return m_bearing_y;
}

void sad::freetype::FixedSizeFont::markTexturesAsUnloaded()
{
    m_texture->IsOnGPU = false;
}

// Why this even exists? It's not like were doing something horrible here
// Also, we encounter bad Linux behaviour here
// static sad::Mutex sad_freetype_font_lock;

void sad::freetype::FixedSizeFont::render(
    const sad::String & s, 
    const sad::Point2D & p, 
    float ratio,
    sad::Font::RenderFlags flags
)
{
    // sad_freetype_font_lock.lock();

    if (!m_on_gpu)
    {
        m_texture->upload();
        m_on_gpu = true;
    }

    sad::String tmp = s;
    tmp.removeAllOccurences("\r");
    sad::StringList list = tmp.split("\n", sad::String::KEEP_EMPTY_PARTS);

    bool previous = false;
    unsigned char prevchar = 0;
    float xbegin = p.x();
    float curx = xbegin;
    float cury = p.y() - m_bearing_y;
    float topoffset = m_height * sad::freetype::Glyph::tan_20_degrees;
    bool italic = ((flags & sad::Font::FRF_Italic) != 0);
    if (!italic)
    {
        topoffset = 0;
    }

    m_texture->bind();
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
            
            g->render(curx, cury, topoffset);
            if ((flags & sad::Font::FRF_Bold) != 0)
            {
                curx += 1.0;
                g->render(curx, cury, topoffset);
                curx += 1.0;
                g->render(curx, cury, topoffset);
            }

            curx += g->AdvanceX;
            prevchar = curchar;
            previous = true;
        }
        cury -= m_builtin_linespacing * ratio;
        curx = xbegin;
        previous = false;
    }

    // sad_freetype_font_lock.unlock();
}

void sad::freetype::FixedSizeFont::fillGeometries(const sad::Font::GeometryRenderData& data, sad::os::GLFontGeometries& geometries, const sad::String & str, const sad::Point2D & p, sad::Font::RenderFlags flags, float ratio)
{
    sad::Vector<double> vertexes;
    sad::Vector<double> texturecoords;

    if (!m_on_gpu)
    {
        m_texture->upload();
        m_on_gpu = true;
    }

    sad::String tmp = str;
    tmp.removeAllOccurences("\r");
    sad::StringList list = tmp.split("\n", sad::String::KEEP_EMPTY_PARTS);

    bool previous = false;
    unsigned char prevchar = 0;
    double xbegin = static_cast<float>(p.x());
    double curx = xbegin;
    double cury = static_cast<float>(p.y() - m_bearing_y);
    double topoffset = m_height * sad::freetype::Glyph::tan_20_degrees;
    bool italic = ((flags & sad::Font::FRF_Italic) != 0);
    if (!italic)
    {
        topoffset = 0;
    }

    for (unsigned int i = 0; i < list.size(); i++)
    {
        for (unsigned int j = 0; j < list[i].size(); j++)
        {
            unsigned char curchar = list[i][j];
            if (previous)
            {
                curx += m_kerning_table[prevchar][curchar];
            }

            sad::freetype::Glyph * g = m_glyphs[curchar];

            g->fillGeometries(curx, cury, topoffset, vertexes, texturecoords);
            if ((flags & sad::Font::FRF_Bold) != 0)
            {
                curx += 1.0;
                g->fillGeometries(curx, cury, topoffset, vertexes, texturecoords);
                curx += 1.0;
                g->fillGeometries(curx, cury, topoffset, vertexes, texturecoords);
            }

            curx += g->AdvanceX;
            prevchar = curchar;
            previous = true;
        }
        cury -= m_builtin_linespacing * ratio;
        curx = xbegin;
        previous = false;
    }

    geometries.append(data.Renderer, m_texture, vertexes, texturecoords, data.OwnColor, data.Color);
}

sad::String sad::freetype::FixedSizeFont::dumpGlyphParameters() const
{
    sad::String result = str(
        fmt::Format("Freetype version: {0}.{1}.{2}\n")
        << FREETYPE_MAJOR
        << FREETYPE_MINOR
        << FREETYPE_PATCH
    );
    for(int i = 0; i < 256; i++)
    {
        result += sad::String::number(i);
        result += ":";
        result += m_glyphs[i]->dumpParametes();
    }
    return result;
}

sad::Texture * sad::freetype::FixedSizeFont::renderToTexture(
    const sad::String & string,
    FT_Library library,
    FT_Face face,
    unsigned int height
)
{
    requestSize(library, face, height);

    sad::String tmp = string;
    tmp.removeAllOccurences("\r");
    tmp.removeAllOccurences("\n");

    sad::Size2D size = this->size(string, 1.0, sad::Font::FRF_None);

    sad::Texture * texture = new sad::Texture();
    texture->width() = static_cast<unsigned int>(ceil(size.Width));
    texture->height() = static_cast<unsigned int>(ceil(size.Height));
    texture->bpp() = 32;
    sad::Vector<sad::uchar>& vdata = reinterpret_cast<sad::Texture::DefaultBuffer*>(texture->Buffer)->Data;
    vdata.resize(texture->width() * texture->height() * 4, 255);

    // Fill alpha byte with 0
    for(unsigned int i = 0; i < size.Height; i++)
    {
        for(unsigned int j = 0; j < size.Width; j++)
        {
            *(texture->pixel(i, j) + 3) = 0;
        }
    }

    sad::freetype::Glyph ** glyphs = new sad::freetype::Glyph*[tmp.size()];
    int y_max = -1; 
    for(unsigned int i = 0; i < tmp.size(); i++)
    {
        glyphs[i] = new sad::freetype::Glyph(face, tmp[i], true);
        y_max = std::max(y_max, static_cast<int>(glyphs[i]->Height));
    }
    // Place glyphs
    bool previous = false;
    unsigned char prevchar = 0;
    unsigned int curx = 0;
    
    for(unsigned int i = 0; i < tmp.size(); i++)
    {
        unsigned char curchar = tmp[i];
        if (previous)
        {
            curx += static_cast<unsigned int>(m_kerning_table[prevchar][curchar]);
        }
        previous = true;	

        sad::freetype::Glyph * g = glyphs[i];

        unsigned int rows = static_cast<unsigned int>(g->TexCoordinateHeight * g->Texture.Height);
        unsigned int cols = static_cast<unsigned int>(g->TexCoordinateWidth  * g->Texture.Width);
        
        for(unsigned int ix = 0; ix < rows; ix++)
        {
            for(unsigned int iy = 0; iy < cols; iy++)
            {
                unsigned char pixel = g->Texture.Pixels[2 * (ix * static_cast<unsigned int>(g->Texture.Width)+ iy)];
                pixel = 255 - pixel;

                unsigned int posy = ix + y_max - static_cast<int>(g->Height);
                unsigned int posx = curx + iy;

                if (posy < texture->height() && posx < texture->width())
                {
                    sad::uchar * tpixel = texture->pixel(posy, posx);
                    tpixel[0] = pixel;
                    tpixel[1] = pixel;
                    tpixel[2] = pixel;
                    tpixel[3] = 255;
                }
            }
        }

        curx += static_cast<unsigned int>(g->AdvanceX);
        prevchar = curchar;
    }

    for(unsigned int i = 0; i < tmp.size(); i++)
    {
        delete glyphs[i];
    }
    delete[] glyphs;

    return texture;
}

sad::Size2D sad::freetype::FixedSizeFont::size(
    const sad::String & s, 
    float ratio,
    sad::Font::RenderFlags flags
)
{
    sad::String tmp = s;
    tmp.removeAllOccurences("\r");
    sad::StringList list = tmp.split("\n", sad::String::KEEP_EMPTY_PARTS);

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
        if ((flags & sad::Font::FRF_Bold) != 0)
        {
            curx += list[i].size() * 2; // 2 is bold font size
        }
        if ((flags & sad::Font::FRF_Italic) != 0)
        {
            curx += m_height * sad::freetype::Glyph::tan_20_degrees;
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
    FT_Library /*library*/,
    FT_Face face, 
    unsigned int height
)
{
     double pixelheight = height;
     double ptheight = pixelheight / 96.0 * 72.0;
     int  pointheight = static_cast<int>(ceil(ptheight)) << 6;

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
            wchar_t c1 = sad::freetype::to_wide_char(static_cast<unsigned int>(i));
            unsigned int index1 = FT_Get_Char_Index(face, c1);
            for(unsigned int j = 0; j < 256; j++)
            {
                wchar_t c2 = sad::freetype::to_wide_char(static_cast<unsigned int>(j));
                unsigned int index2 = FT_Get_Char_Index(face, c2);
                FT_Get_Kerning(face, index1, index2, FT_KERNING_DEFAULT, &kerning);
                m_kerning_table[i][j] = static_cast<float>(kerning.x >> 6);
            }
        }
    }
    else
    {
        std::fill_n(&(m_kerning_table[0][0]), 256 * 256, 0.0f);
    }
}

#include "packer.h"

#include <functional>
#include <algorithm>

#include "../../include/3rdparty/framepacker/framepacker.hpp"
#include "../../include/texture.h"
#include "../../include/imageformats/bmploader.h"

// ============================  sad::freetype::Packer::T methods ============================

#define PADDING 2
sad::freetype::Packer::T::T() :
    m_width(0.0),
    m_height(0.0),
    m_t(nullptr)
{

}

sad::freetype::Packer::T::T(sad::freetype::Glyph* t) :
    m_width(0.0),
    m_height(0.0),
    m_t(t)
{
    m_width = t->Width + PADDING;
    m_height = t->Height + PADDING;
}

int sad::freetype::Packer::T::width() const
{
    return static_cast<int>(m_width);
}

int sad::freetype::Packer::T::height() const
{
    return static_cast<int>(m_height);
}

int sad::freetype::Packer::T::area() const
{
    return static_cast<int>(m_width * m_height);
}

void sad::freetype::Packer::T::resize(double w, double h)
{
    m_width = w;
    m_height = h;
}

// ReSharper disable once CppMemberFunctionMayBeConst
int sad::freetype::Packer::T::pixel(int, int)
{
    return static_cast<int>(area());
}

// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppMemberFunctionMayBeStatic
void sad::freetype::Packer::T::pixel(int x, int y, int p)
{

}

// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppMemberFunctionMayBeStatic
bool sad::freetype::Packer::T::is_transparent(int x, int y)
{
    return false;
}

// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppMemberFunctionMayBeStatic
void sad::freetype::Packer::T::copy_from(
    sad::freetype::Packer::T block,
    int x,
    int y,
    int width,
    int height,
    int px,
    int py
)
{
    m_t = block.m_t;
    m_t->TextureRectangle = sad::Rect2D(x + px, y + py, x + px + width, y + py + height);
}

// ============================  sad::freetype::Packer::Packer methods ============================

sad::freetype::Texture* sad::freetype::Packer::pack(sad::freetype::Glyph* glyphs[256])
{
    typedef framepacker::packer<sad::freetype::Packer::T, false, false> packer_type;
    packer_type packer;
    packer.padding = 0;
    packer.alpha_trim = false;
    packer.allow_rotate = false;
    packer.comparer = packer_type::compare_area;

    sad::freetype::Packer::T* result = new sad::freetype::Packer::T();
    packer_type::texture_type result_ptr(result);

    packer_type::texture_coll_type packed;
    packer_type::texture_coll_type failed;
    for (size_t i = 0; i < 256; i++) {
        sad::freetype::Glyph* t = glyphs[i];
        sad::freetype::Packer::T* img = new sad::freetype::Packer::T(t);

        packer.add(
            sad::String::number(i, 16),
            packer_type::texture_type(img)
        );
    }
    packer.pack(result_ptr, packed, failed);

    std::function<unsigned int(double)> nextPOT = [](double value) -> unsigned int {
        unsigned int  size = 1;
        while (size < value)
        {
            size = size << 1;
        }
        return size;
    };

    unsigned int width = nextPOT(result->width());
    unsigned int height = nextPOT(result->height());
    unsigned int wh = std::max(width, height);

    sad::freetype::Texture*  t = new sad::freetype::Texture();
    t->Width = static_cast<float>(wh);
    t->Height = static_cast<float>(wh);
    t->Pixels.resize(2 * wh * wh, 0);
    for (int  c = 0; c < 256; c++)
    {
        const sad::Rect2D& texRect = glyphs[c]->TextureRectangle;
        
        int p0x = static_cast<int>(texRect.p0().x());
        int p0y = static_cast<int>(texRect.p0().y());
        int src_tc_width = static_cast<int>(glyphs[c]->Width);
        int src_tc_height = static_cast<int>(glyphs[c]->Height);


        if (src_tc_width != 0  && src_tc_height != 0 && (glyphs[c]->Data != nullptr))
        {
            FT_BitmapGlyph bitmap_glyph = reinterpret_cast<FT_BitmapGlyph>(glyphs[c]->Data);
            FT_Bitmap & bitmap = bitmap_glyph->bitmap;

            // Top part
            t->copyPixel(p0x, p0y, 0, 0, bitmap);
            t->copyRow(p0x + 1, p0y, 0, 0, src_tc_width, bitmap);
            t->copyPixel(p0x + 1 + src_tc_width, p0y, src_tc_width - 1, 0, bitmap);

            // Middle
            t->copySubImage(p0x, p0y + 1, 0, 0, 1, src_tc_height, bitmap);
            t->copySubImage(p0x + 1, p0y + 1, 0, 0, src_tc_width, src_tc_height, bitmap);
            t->copySubImage(p0x + 1 + src_tc_width, p0y + 1, src_tc_width - 1, 0, 1, src_tc_height, bitmap);

            // Bottom part
            t->copyPixel(p0x, p0y + 1 + src_tc_height, 0, src_tc_height - 1, bitmap);
            t->copyRow(p0x + 1, p0y + 1 + src_tc_height, 0, src_tc_height - 1, src_tc_width, bitmap);
            t->copyPixel(p0x + 1 + src_tc_width, p0y + 1 + src_tc_height, src_tc_width - 1, src_tc_height - 1, bitmap);
        }

        double dx = static_cast<double>(p0x + 1.0) / static_cast<double>(wh);
        double dy = static_cast<double>(p0y + 1.0) / static_cast<double>(wh);

        double dx2 = static_cast<double>(p0x + 1.0 + src_tc_width) / static_cast<double>(wh);
        double dy2 = static_cast<double>(p0y + 1.0 + src_tc_height) / static_cast<double>(wh);

        glyphs[c]->TextureRectangle = sad::Rect2D(dx, dy, dx2, dy2);
    }
    return t;
}

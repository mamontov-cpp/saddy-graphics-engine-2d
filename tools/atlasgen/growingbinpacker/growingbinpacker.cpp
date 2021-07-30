#include "growingbinpacker.h"

#include "../fullsearchpacker/imagearranger.h"

#include "../../../include/3rdparty/framepacker/framepacker.hpp"

// ============================  growingbinpacker::GrowingBinPacker::T methods ============================

growingbinpacker::GrowingBinPacker::T::T() :
m_width(0.0),
m_height(0.0),
m_t(nullptr)
{
    
}

growingbinpacker::GrowingBinPacker::T::T(Texture* t, double pad_x, double pad_y) :
m_width(0.0),
m_height(0.0),
m_t(t)
{
    m_width = t->Image.width() + pad_x;
    m_height = t->Image.height() + pad_y;
}

double growingbinpacker::GrowingBinPacker::T::width() const
{
    return m_width;
}

double growingbinpacker::GrowingBinPacker::T::height() const
{
    return m_height;
}

double growingbinpacker::GrowingBinPacker::T::area() const
{
    return m_width * m_height;
}

void growingbinpacker::GrowingBinPacker::T::resize(double w, double h)
{
    m_width = w;
    m_height = h;
}

// ReSharper disable once CppMemberFunctionMayBeConst
int growingbinpacker::GrowingBinPacker::T::pixel(int, int )
{
    return static_cast<int>(area());
}

// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppMemberFunctionMayBeStatic
void growingbinpacker::GrowingBinPacker::T::pixel(int x, int y, int p)
{
    
}

// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppMemberFunctionMayBeStatic
bool growingbinpacker::GrowingBinPacker::T::is_transparent(int x, int y)
{
    return false;
}

// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppMemberFunctionMayBeStatic
void growingbinpacker::GrowingBinPacker::T::copy_from(
        growingbinpacker::GrowingBinPacker::T block,
        int x, 
        int y,
        int width,
        int height,
        int px, 
        int py
)
{
    block.m_t->TextureRectangle = QRectF(QPointF(x + px,y + py), QSizeF(width, height));
}


// ============================  growingbinpacker::GrowingBinPacker methods ============================

growingbinpacker::GrowingBinPacker::GrowingBinPacker()
{
    
}

void growingbinpacker::GrowingBinPacker::pack(Atlas& atlas, QImage*& image)
{
    bool add_pixel = (*m_options)["add-pixel"].toBool();
    double padx = 0, pady = 0;
    if (add_pixel)
    {
        padx = 2;
        pady = 2;
    }
    typedef framepacker::packer<growingbinpacker::GrowingBinPacker::T, false, false> packer_type;
    packer_type packer;
    packer.padding = 0;
    packer.alpha_trim = false;
    packer.allow_rotate = false;
    packer.comparer =  packer_type::compare_area;
    
    growingbinpacker::GrowingBinPacker::T* result = new growingbinpacker::GrowingBinPacker::T();
    packer_type::texture_type result_ptr(result);

    packer_type::texture_coll_type packed;
    packer_type::texture_coll_type failed;
    for(size_t i = 0; i < atlas.textures().size();  i++) {
        Texture* t = atlas.textures()[i];
        growingbinpacker::GrowingBinPacker::T* img = new growingbinpacker::GrowingBinPacker::T(t, padx, pady);

        packer.add(
            t->Name.toStdString(), 
            packer_type::texture_type(img)
        );
    }
    packer.pack(result_ptr, packed, failed);

    unsigned int width = fullsearchpacker::ImageArranger::nextPOT(result->width());
    unsigned int height = fullsearchpacker::ImageArranger::nextPOT(result->height());
    unsigned int wh = std::max(width, height);

    image = new QImage(wh, wh, QImage::Format_ARGB32);
    image->fill(QColor(255, 255, 255, 0));

    this->copyImages(atlas, image);
    if (add_pixel)
    {
        for(size_t i = 0; i < atlas.textures().size(); i++)
        {
            Texture* t = atlas.textures()[i];
            QSizeF oldsz = t->TextureRectangle.size();
            t->TextureRectangle.setSize(QSizeF(oldsz.width() - padx, oldsz.height() - pady));
        }
    }
}

growingbinpacker::GrowingBinPacker::~GrowingBinPacker()
{
    
}

#include "gui/resourcetreewidget/resourcecache.h"
#include "gui/resourcetreewidget/resourcetreewidget.h"
#include "gui/resourcetreewidget/cell.h"
#include "gui/resourcetreewidget/defaultimage.h"

#include "../../qstdstring.h"

#include <renderer.h>

#include <resource/resource.h>
#include <resource/tree.h>

#include <freetype/font.h>
#include <texturemappedfont.h>
#include <db/custom/customschema.h>

gui::resourcetreewidget::ResourceCache::ResourceCache() : m_parent(nullptr)
{
    
}

gui::resourcetreewidget::ResourceCache::~ResourceCache()
{
    
}

void gui::resourcetreewidget::ResourceCache::setParent(ResourceTreeWidget * parent)
{
    m_parent = parent;
    m_resource_cache.clear();
}

gui::resourcetreewidget::ResourceTreeWidget* gui::resourcetreewidget::ResourceCache::parent() const
{
    return m_parent;
}

const QImage& gui::resourcetreewidget::ResourceCache::imageForResource(const QString & resource_name)
{
    // A name for resource
    if (m_resource_cache.contains(resource_name))
    {
        return m_resource_cache[resource_name];
    }
    // Try fetch result
    QImage result;
    sad::resource::Tree * tree = sad::Renderer::ref()->tree(Q2STDSTRING(m_parent->tree()).c_str()); 
    sad::resource::Resource * resource = tree->root()->resource(Q2STDSTRING(resource_name).c_str());

    // Handle all resource rendering strings here
    bool handled = false;
    if (resource)
    {
        sad::ClassMetaData* meta = resource->metaData();
        if (meta->name() == "sad::freetype::Font")
        {
            sad::freetype::Font * font = static_cast<sad::freetype::Font*>(resource);
            sad::Texture * texture  = font->renderToTexture("Test", 20);
            result = QImage(texture->data(), texture->width(), texture->height(), QImage::Format_ARGB32).copy();
            delete texture;
            handled = true;
        }
        if (meta->name() == "sad::TextureMappedFont" && !handled)
        {
            sad::TextureMappedFont * font = static_cast<sad::TextureMappedFont*>(resource);
            sad::Texture * texture  = font->renderToTexture("Test");
            result = QImage(texture->data(), texture->width(), texture->height(), QImage::Format_ARGB32).copy();
            delete texture;
            handled = true;
        }
        if (meta->name() == "sad::Sprite2D::Options" && !handled)
        {
            sad::Sprite2D::Options * options = static_cast<sad::Sprite2D::Options*>(resource);
            resourcetreewidget::ResourceCache::createImageForTextureAtlasEntry(
                result, 
                *options, 
                tree
            );
            handled = true;
        }
        if (meta->name() == "sad::db::custom::Schema" && !handled)
        {
            sad::db::custom::Schema* schema = static_cast<sad::db::custom::Schema*>(resource);
            result = this->imageForResource(STD2QSTRING(schema->treeItemName())).copy();

            handled = true;
        }
        if (meta->name() == "sad::Texture" && !handled)
        {
            sad::Texture * tex = static_cast<sad::Texture*>(resource);
            resourcetreewidget::ResourceCache::createImageForTexture(
                result, 
                tex
            );
            handled = true;
        }
    }

    if (!handled)
    {
        this->createDefaultImage(result);
    }
    normalizeImage(result);
    m_resource_cache.insert(resource_name, result);
    return m_resource_cache[resource_name];
}

void gui::resourcetreewidget::ResourceCache::createImageForTextureAtlasEntry(
    QImage & im,
    const sad::Sprite2D::Options & options,
    sad::resource::Tree * tree
)
{
    sad::Texture * source = tree->get<sad::Texture>(options.Texture);
    
    sad::Rect2I tmp = sad::_(options.TextureRectangle);
    for (int i=0;i<4;i++)
    {
            if (tmp[i].x() < 0) tmp[i].setX(0);
            if (tmp[i].y() < 0) tmp[i].setY(0);
            if (tmp[i].x() > source->width()-1) tmp[i].setX(source->width() - 1);
            if (tmp[i].y() > source->height()-1) tmp[i].setY(source->height() - 1);
    }
    int minx = static_cast<int>(tmp[0].x());
    int maxx = static_cast<int>(tmp[2].x());
    int miny = static_cast<int>(tmp[0].y());
    int maxy = static_cast<int>(tmp[2].y());
    if (maxx < minx)
    {
            std::swap(maxx, minx);
    }
    if (maxy < miny)
    {
            std::swap(maxy, miny);
    }
    
    int width = maxx - minx;
    int height = maxy - miny;

    if (width > 0 && height > 0)
    {
        sad::Texture * result = new sad::Texture();
        result->width() = static_cast<float>(width);
        result->height() = static_cast<float>(height);	
        result->bpp() = 32;
        result->Format = sad::Texture::InternalFormat::SFT_R8_G8_B8_A8;
    
        int bypp = result->bpp() / 8;
        int sbypp = source->bpp() / 8;
        sad::Vector<sad::uchar>& vdata = reinterpret_cast<sad::Texture::DefaultBuffer*>(result->Buffer)->Data;
        vdata.resize(width * height * bypp);

        for (int row = miny; row < maxy; row++)
        {
            sad::uchar * srcrow = source->pixel(row, minx);
            sad::uchar * destrow = result->pixel(row - miny, 0);
            for(int col = 0; col < width; col++)
            {
                const sad::uchar * srcpix = srcrow + col * sbypp;
                sad::uchar * destpix = destrow + col * bypp;
                gui::resourcetreewidget::ResourceCache::copyPixel(destpix, srcpix, static_cast<unsigned int>(source->Format), sbypp);
            }
        }

        QImage t(result->data(), result->width(), result->height(), QImage::Format_ARGB32);
        im = t.copy();

        delete result;
    }
    else
    {
        im = QImage().copy();
    }
}

void gui::resourcetreewidget::ResourceCache::createImageForTexture(
    QImage & im,
    sad::Texture * tex
)
{        
    im = QImage(tex->width(), tex->height(), QImage::Format_ARGB32).copy();
    uchar* destpixels = im.bits();
    const sad::uchar * srcpixels = tex->data();
    size_t bypp = tex->bpp() / 8;
    size_t imgsize = tex->width() * tex->height();
    for(int pos = 0; pos < imgsize; pos += 1)
    {
        const sad::uchar * srcpix = srcpixels + pos * bypp;
        uchar * destpix = destpixels + pos * 4; // We have four bytes per pixel in destination

        gui::resourcetreewidget::ResourceCache::copyPixel(destpix, srcpix, static_cast<unsigned int>(tex->Format), bypp);
    }
}

void gui::resourcetreewidget::ResourceCache::createDefaultImage(QImage & im)
{
    im = QImage(
        gui::resourcetreewidget::DefaultImage::Data, 
        gui::resourcetreewidget::DefaultImage::Width, 
        gui::resourcetreewidget::DefaultImage::Height, 
        QImage::Format_ARGB32
    ).copy();
}

void gui::resourcetreewidget::ResourceCache::clear()
{
    m_resource_cache.clear();
}

void gui::resourcetreewidget::ResourceCache::normalizeImage(QImage & im)
{
    if (im.width() > gui::resourcetreewidget::Cell::ImageWidth 
        || im.height() > gui::resourcetreewidget::Cell::ImageHeight)
    {
        if (static_cast<unsigned int>(im.height()) != 0)
        {
            float ratio = static_cast<float>(im.height()) / im.width();
            int width, height;
            if (im.width() > im.height() 
                && ratio * gui::resourcetreewidget::Cell::ImageWidth <= gui::resourcetreewidget::Cell::ImageHeight)
            {
                width = gui::resourcetreewidget::Cell::ImageWidth;
                height = static_cast<int>(ratio * static_cast<float>(width));
            }
            else
            {
                height = gui::resourcetreewidget::Cell::ImageHeight;
                width = static_cast<int>(static_cast<float>(height) / ratio);
            }
            im = im.scaled(width, height,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        }
    }
}

void gui::resourcetreewidget::ResourceCache::copyPixel(
    unsigned char* destination_pixel,
    const unsigned char* source_pixel,
    unsigned int format,
    int sbypp
)
{
    switch (static_cast<sad::Texture::InternalFormat>(format))
    {
    case sad::Texture::InternalFormat::SFT_R8_G8_B8_A8:
        destination_pixel[0] = source_pixel[2];
        destination_pixel[1] = source_pixel[1];
        destination_pixel[2] = source_pixel[0];
        if (sbypp == 4)
            destination_pixel[3] = source_pixel[3];
        else
            destination_pixel[3] = 255;
        break;
    case sad::Texture::InternalFormat::SFT_R5_G6_B5:
        {
            unsigned short px = *reinterpret_cast<const unsigned short*>(source_pixel);
            unsigned short red = (px & 63488) >> 11;
            unsigned short green = (px & 2016) >> 5;
            unsigned short blue = (px & 31);
            destination_pixel[0] = static_cast<unsigned char>(static_cast<float>(blue) / 31.0 * 255.0);
            destination_pixel[1] = static_cast<unsigned char>(static_cast<float>(green) / 63.0 * 255.0);
            destination_pixel[2] = static_cast<unsigned char>(static_cast<float>(red) / 31.0 * 255.0);
            destination_pixel[3] = 255;
        }
        break;
    case sad::Texture::InternalFormat::SFT_R4_G4_B4_A4:
        {
            unsigned short px = *reinterpret_cast<const unsigned short*>(source_pixel);
            unsigned short red = (px & 61440) >> 12;
            unsigned short green = (px & 3840) >> 8;
            unsigned short blue = (px & 240) >> 4;
            unsigned short alpha = (px & 15);
            destination_pixel[0] = static_cast<unsigned char>(static_cast<float>(blue) / 15.0 * 255.0);
            destination_pixel[1] = static_cast<unsigned char>(static_cast<float>(green) / 15.0 * 255.0);
            destination_pixel[2] = static_cast<unsigned char>(static_cast<float>(red) / 15.0 * 255.0);
            destination_pixel[3] = static_cast<unsigned char>(static_cast<float>(alpha) / 15.0 * 255.0);
        }
        break;
    case sad::Texture::InternalFormat::SFT_R3_G3_B2:
        {
            unsigned char red = (((*source_pixel) & 224) >> 5);
            unsigned char green = (((*source_pixel) & 28) >> 2);
            unsigned char blue = ((*source_pixel) & 3);
            destination_pixel[0] = static_cast<unsigned char>(static_cast<float>(blue) / 3.0 * 255.0);
            destination_pixel[1] = static_cast<unsigned char>(static_cast<float>(green) / 7.0 * 255.0);
            destination_pixel[2] = static_cast<unsigned char>(static_cast<float>(red) / 7.0 * 255.0);
            destination_pixel[3] = 255;
        }
        break;
    default: // Don't do changes if format is invalid
        break;
    };
}

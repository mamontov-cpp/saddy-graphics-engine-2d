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

gui::resourcetreewidget::ResourceCache::ResourceCache() : m_parent(NULL)
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

const QImage& gui::resourcetreewidget::ResourceCache::imageForResource(const QString & resourcename)
{
    // A name for resource
    if (m_resource_cache.contains(resourcename))
    {
        return m_resource_cache[resourcename];
    }
    // Try fetch result
    QImage result;
    sad::resource::Tree * tree = sad::Renderer::ref()->tree(Q2STDSTRING(m_parent->tree()).c_str()); 
    sad::resource::Resource * resource = tree->root()->resource(Q2STDSTRING(resourcename).c_str());

    // Handle all resource rendering strings here
    bool handled = false;
    if (resource)
    {
        sad::ClassMetaData* meta = resource->metaData();
        if (meta->name() == "sad::freetype::Font" && !handled)
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
    m_resource_cache.insert(resourcename, result);
    return m_resource_cache[resourcename];
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

    if (width > 0 && height > 0) {
        sad::Texture * result = new sad::Texture();
        result->width() = static_cast<float>(width);
        result->height() = static_cast<float>(height);	
        result->bpp() = source->bpp();
    
        int bypp = result->bpp() / 8;
        result->vdata().resize(width * height * bypp);

        for (int row = miny; row < maxy; row++)
        {
            sad::uchar * srcrow = source->pixel(row, minx);
            sad::uchar * destrow = result->pixel(row - miny, 0);
            for(int col = 0; col < width; col++)
            {
                const sad::uchar * srcpix = srcrow + col * bypp;
                sad::uchar * destpix = destrow + col * bypp;
                destpix[0] = srcpix[2];
                destpix[1] = srcpix[1];
                destpix[2] = srcpix[0];
                destpix[3] = srcpix[3];			
            }
        }
        if (bypp == 4)
        {
            QImage t(result->data(), result->width(), result->height(), QImage::Format_ARGB32);
            im = t.copy();
        }
        if (bypp == 3)
        {
            QImage t(result->data(), result->width(), result->height(), QImage::Format_RGB888);
            im = t.copy();
        }	
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
    for(int pos = 0; pos < tex->vdata().size(); pos += bypp)
    {
        const sad::uchar * srcpix = srcpixels + pos;
        uchar * destpix = destpixels + pos;

        destpix[0] = srcpix[2];
        destpix[1] = srcpix[1];
        destpix[2] = srcpix[0];
        destpix[3] = srcpix[3];	
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

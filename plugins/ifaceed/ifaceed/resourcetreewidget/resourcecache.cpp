#include "resourcetreewidget/resourcecache.h"
#include "resourcetreewidget/resourcetreewidget.h"
#include "resourcetreewidget/cell.h"
#include "resourcetreewidget/defaultimage.h"

#include <renderer.h>

#include <resource/resource.h>
#include <resource/tree.h>

#include <freetype/font.h>
#include <texturemappedfont.h>

#include <cassert>

resourcetreewidget::ResourceCache::ResourceCache() : m_parent(NULL)
{
	
}

resourcetreewidget::ResourceCache::~ResourceCache()
{
	
}

void resourcetreewidget::ResourceCache::setParent(ResourceTreeWidget * parent)
{
	m_parent = parent;
	m_resource_cache.clear();
}

ResourceTreeWidget* resourcetreewidget::ResourceCache::parent() const
{
	return m_parent;
}

const QImage& resourcetreewidget::ResourceCache::imageForResource(const QString & resourcename)
{
	// A name for resource
	if (m_resource_cache.contains(resourcename))
	{
		return m_resource_cache[resourcename];
	}
	// Try fetch result
	QImage result;
	sad::resource::Tree * tree = sad::Renderer::ref()->tree(m_parent->tree().toStdString().c_str()); 
	sad::resource::Resource * resource = tree->root()->resource(resourcename.toStdString().c_str());

	// Handle all resource rendering strings here
	bool handled = false;
	if (resource)
	{
		if (resource->metaData()->name() == "sad::freetype::Font" && !handled)
		{
			sad::freetype::Font * font = (sad::freetype::Font*)resource;
			sad::Texture * texture  = font->renderToTexture("Test", 20);
			result = QImage(texture->data(), texture->width(), texture->height(), QImage::Format_ARGB32).copy();
			delete texture;
			handled = true;
		}
		if (resource->metaData()->name() == "sad::TextureMappedFont" && !handled)
		{
			sad::TextureMappedFont * font = (sad::TextureMappedFont*)resource;
			sad::Texture * texture  = font->renderToTexture("Test");
			result = QImage(texture->data(), texture->width(), texture->height(), QImage::Format_ARGB32).copy();
			delete texture;
			handled = true;
		}
		if (resource->metaData()->name() == "sad::Sprite2D::Options" && !handled)
		{
			sad::Sprite2D::Options * options = (sad::Sprite2D::Options*)resource;
			resourcetreewidget::ResourceCache::createImageForTextureAtlasEntry(
				result, 
				*options, 
				tree
			);
			handled = true;
		}
		if (resource->metaData()->name() == "sad::Texture" && !handled)
		{
			sad::Texture * tex = (sad::Texture*)resource;
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

void resourcetreewidget::ResourceCache::createImageForTextureAtlasEntry(
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
	int minx = (int)tmp[0].x();
	int maxx = (int)tmp[2].x();
	int miny = (int)tmp[0].y();
	int maxy = (int)tmp[2].y();
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
	
	sad::Texture * result = new sad::Texture();
	result->width() = (float)width;
	result->height() = (float)height;	
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

void resourcetreewidget::ResourceCache::createImageForTexture(
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

void resourcetreewidget::ResourceCache::createDefaultImage(QImage & im)
{
	im = QImage(
		resourcetreewidget::DefaultImage::Data, 
		resourcetreewidget::DefaultImage::Width, 
		resourcetreewidget::DefaultImage::Height, 
		QImage::Format_ARGB32
	).copy();
}

void resourcetreewidget::ResourceCache::normalizeImage(QImage & im)
{
	if (im.width() > resourcetreewidget::Cell::ImageWidth 
		|| im.height() > resourcetreewidget::Cell::ImageHeight)
	{
		if ((unsigned int)(im.height()) != 0)
		{
			float ratio = ((float)im.height()) / im.width();
			int width, height;
			if (im.width() > im.height() 
				&& ratio * resourcetreewidget::Cell::ImageWidth <= resourcetreewidget::Cell::ImageHeight)
			{
				width = resourcetreewidget::Cell::ImageWidth;
				height = (int)(ratio * (float)width);
			}
			else
			{
				height = resourcetreewidget::Cell::ImageHeight;
				width = (int)((float)height / ratio);
			}
			im = im.scaled(width, height,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		}
	}
}

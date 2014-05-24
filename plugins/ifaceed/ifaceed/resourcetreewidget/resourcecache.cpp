#include "resourcetreewidget/resourcecache.h"
#include "resourcetreewidget/resourcetreewidget.h"
#include "resourcetreewidget/cell.h"

#include <renderer.h>

#include <resource/resource.h>
#include <resource/tree.h>

#include <freetype/font.h>

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
			result = QImage(texture->data(), texture->width(), texture->height(), QImage::Format_ARGB32);
			delete texture;
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

void resourcetreewidget::ResourceCache::normalizeImage(QImage & im)
{
	if (im.width() > resourcetreewidget::Cell::Width 
		|| im.height() > resourcetreewidget::Cell::Height)
	{
		if ((unsigned int)(im.height()) != 0)
		{
			float ratio = im.height() / im.width();
			int width, height;
			if (im.width() > resourcetreewidget::Cell::Width)
			{
				width = resourcetreewidget::Cell::Width;
				height = (int)(ratio * (float)width);
			}
			else
			{
				height = resourcetreewidget::Cell::Height;
				width = (int)((float)height / ratio);
			}
			im = im.scaled(width, height,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		}
	}
}

void resourcetreewidget::ResourceCache::createDefaultImage(QImage & im)
{
	
}

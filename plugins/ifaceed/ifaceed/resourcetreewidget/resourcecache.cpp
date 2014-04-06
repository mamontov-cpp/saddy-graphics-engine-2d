#include "resourcetreewidget/resourcecache.h"
#include "resourcetreewidget/resourcetreewidget.h"

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
	this->createDefaultImage(result);
	m_resource_cache.insert(resourcename, result);
	return m_resource_cache[resourcename];
}


void resourcetreewidget::ResourceCache::createDefaultImage(QImage & im)
{
	
}

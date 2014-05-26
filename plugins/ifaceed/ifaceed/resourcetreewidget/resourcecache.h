/*! \file resourcecache.h
	\author HiddenSeeker

	A cache for resources as images for displaying it in tables
 */
#pragma once
#include <QImage>
#include <QHash>

class ResourceTreeWidget;

namespace resourcetreewidget
{

class ResourceCache
{
public:
	/*! Constructs new resulting cache for item
	 */
	ResourceCache();
	/*! Destroys a resulting cache for item
	 */
	~ResourceCache();
	/*! Sets new parent widget fo a resource cache
	 */
	void setParent(ResourceTreeWidget * parent);	
	/*! Returns parent element for a cache
		\return a parent widget
	 */
	ResourceTreeWidget* parent() const;
	/*! Returns image for resource
		\param[in] resourcename a name for resource
		\return image, which should be rendered on resources
	 */
	const QImage& imageForResource(const QString & resourcename);
	/*! Creates default image for specific resource
		\param[in, out] a default image for resource
	 */
	void createDefaultImage(QImage & im);
protected:
	/*! Makes image size less or equal to a cell proportions
		\param[in, out] image 
	 */
	void normalizeImage(QImage & im);
	/*! A cache for resources and data
	 */
	QHash<QString, QImage> m_resource_cache;
	/*! A parent widget 
	 */
	ResourceTreeWidget * m_parent;
};
	
}

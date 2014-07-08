/*! \file resourcecache.h
	\author HiddenSeeker

	A cache for resources as images for displaying it in tables
 */
#pragma once
#include <QImage>
#include <QHash>

#include <sprite2d.h>

namespace gui
{

namespace resourcetreewidget
{

class ResourceTreeWidget;

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
	/*! Creates image for a texture atlas entry
		\param[in, out] im image, wheres should be placed texture atlas entry
		\param[in] options options, where data about linked texture and coordinates are stored
		\param[in] tree a tree, for a texture data
	 */
	static void createImageForTextureAtlasEntry(
		QImage & im,
		const sad::Sprite2D::Options & options,
		sad::resource::Tree * tree
	);
	/*! Creates image for a texture
		\param[in, out] im image, wheres should be placed texture 
		\param[in] tex texture
	 */ 
	static void createImageForTexture(
		QImage & im,
		sad::Texture * tex
	);
	/*! Creates default image for specific resource
		\param[in, out] im a default image for resource
	 */
	static void createDefaultImage(QImage & im);
protected:
	/*! Makes image size less or equal to a cell proportions
		\param[in, out] im an image for normalizing 
	 */
	void normalizeImage(QImage & im);
	/*! A cache for resources and data
	 */
	QHash<QString, QImage> m_resource_cache;
	/*! A parent widget 
	 */
	gui::resourcetreewidget::ResourceTreeWidget * m_parent;
};
	
}

}

/*! \file resourcecache.h
    

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
        \param[in] resource_name a name for resource
        \return image, which should be rendered on resources
     */
    const QImage& imageForResource(const QString & resource_name);
    /*! Creates image for a texture atlas entry
        \param[in,out] im image, wheres should be placed texture atlas entry
        \param[in] options options, where data about linked texture and coordinates are stored
        \param[in] tree a tree, for a texture data
     */
    static void createImageForTextureAtlasEntry(
        QImage & im,
        const sad::Sprite2D::Options & options,
        sad::resource::Tree * tree
    );
    /*! Creates image for a texture
        \param[in,out] im image, wheres should be placed texture 
        \param[in] tex texture
     */ 
    static void createImageForTexture(
        QImage & im,
        sad::Texture * tex
    );
    /*! Creates default image for specific resource
        \param[in,out] im a default image for resource
     */
    static void createDefaultImage(QImage & im);
    /*!
     * Clears resource cache
     */
    void clear();
protected:
    /*! Makes image size less or equal to a cell proportions
        \param[in,out] im an image for normalizing 
     */
    void normalizeImage(QImage & im);
    /*! Copies pixel from source to destination, transforming it to
        32-bit RGBA format, with respect to source texture format
        \param[out] destination_pixel location of destination pixel
        \param[in]  source_pixel  location of source pixel
        \param[in]  format  format of source texture (sad::Texture::InternalFormat value)
        \param[in]  sbypp   amount of bytes per pixel in source texture
     */
    static void copyPixel(
        unsigned char* destination_pixel,
        const unsigned char* source_pixel,
        unsigned int format,
        int sbypp
    );
    /*! A cache for resources and data
     */
    QHash<QString, QImage> m_resource_cache;
    /*! A parent widget 
     */
    gui::resourcetreewidget::ResourceTreeWidget * m_parent;
};
    
}

}

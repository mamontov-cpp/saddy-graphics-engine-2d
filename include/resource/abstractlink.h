/*! \file resource/abstractlink.h
    
    
    Contains definition of class AbstractLink.

    An abstract link links resources from resource database to an objects, also
    handling reloading of objects.
 */
#pragma once
#include "../sadstring.h"
#include "../renderer.h"

namespace sad
{

namespace resource
{
class Resource;
class Tree;

/*! \class AbstractLink

    An abstract link links resources from resource database to an objects, also
    handling reloading of objects.
 */
class AbstractLink  
{	
public:	
    /*! Constructs new link by resource type
        \param[in] resource_type a basic resource type
     */
    AbstractLink(const sad::String & resource_type);
    /*! This class can be inherited 
     */
    virtual ~AbstractLink();
    /*! Attach a link to specified resource
        \param[in] r a new resource, which links should be attached to
     */
    virtual void attach(sad::resource::Resource* r);
    /*! Sets resource link to object to nullptr
     */
    void detach();
    /*! Lazily fetches a resource
        \return fetched resource
     */
    sad::resource::Resource* resource() const;
    /*! Sets path to an abstract link
        \param[in] path path to a linked resource
     */
    void setPath(sad::String path);
    /*! Returns path to a linked resource
        \return path to a linked resource
     */
    const sad::String & path() const;
    /*! Returns linked tree's name
        \return name of tree
     */
    const sad::String & treeName() const;
    /*! Sets a tree for a link
        \param[in] tree a tree for link
     */
    void setTree(sad::resource::Tree * tree);
    /*! Returns tree, which link references to
        \return tree for a link
     */
    sad::resource::Tree* tree() const;
    /*! Sets a tree for a link, defined by renderer, where it's stored 
        and tree name
        \param[in] r renderer
        \param[in] tree_name a tree name
     */
    void setTree(sad::Renderer * r, const sad::String& tree_name = "");
    /*! Whether link depends on renderer
        \return whether link depends on renderer
     */ 
    bool dependsOnRenderer() const
    {
        return m_render_dependent;
    }
    /*! Returns renderer, which link depends upon
        \return renderer
     */
    sad::Renderer * renderer() const;
    /*! Sets new renderer for a link
        \param[in] r renderer a renderer
     */
    void setRenderer(sad::Renderer * r);
protected:
    /*! Whether we should notify resource, that we are gone
     */
    bool m_notify_resource;
    /*!  A path to abstract link
     */
    sad::String m_path;
    /*!  A type of resource
     */
    sad::String m_resource_type;
    /*!  A resource cache
     */
    sad::resource::Resource* m_resource;
    /*! A tree, where resource is stored
     */
    sad::resource::Tree *   m_tree;
    /*! A source renderer for a link
     */
    sad::Renderer * m_renderer;
    /*! Sets tree name, where tree should be located
     */
    sad::String m_tree_name;
    /*! Tests, whether link depends on renderer or on a tree
     */
    bool m_render_dependent;
};

}

}
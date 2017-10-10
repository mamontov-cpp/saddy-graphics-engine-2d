/*! \file db/dbstronglink.h
    
    
    Describes a strong link to database object, used to link object from a database, storing a reference to it.
 */ 
#pragma once
#include "../sadstring.h"
#include "../db/dbtypename.h"
#include "link.h"

namespace sad
{

namespace resource
{
/*! A strong link to object, with reference counting
 */
template<
    typename T
>
class StrongLink
{
public:
    /*! Constructs new strong link for database objects
     */
    StrongLink() : m_cached_object(NULL), m_changed(true)
    {
        
    }
    /*! Constructs new link via copying
        \param[in] o other object
     */ 
    StrongLink(const sad::resource::StrongLink<T>& o) : m_cached_object(NULL), m_changed(true), m_link(o.m_link)
    {
        
    }
    /*! Copies an inner state for object from source
        \param[in] o object
        \return self-reference
     */
    sad::resource::StrongLink<T>& operator=(const sad::resource::StrongLink<T>& o)
    {
        if (m_cached_object)
        {
            m_cached_object->delRef();
        }
        m_link = o.m_link;
        m_cached_object = NULL;
        m_changed = true;
        return *this;
    }
    /*! Destroys strong link
     */ 
    virtual ~StrongLink()
    {
        if (m_cached_object)
        {
            m_cached_object->delRef();
        }
    }

    /*! Attach a link to specified resource
        \param[in] r a new resource, which links should be attached to
    */
    virtual void attach(T* r)
    {
        if (m_cached_object)
        {
            m_cached_object->delRef();
        }
        m_changed = false;
        m_cached_object = r;
        m_link.attach(r);
        if (r)
        {
            r->addRef();
        }
    }
    /*! Sets resource link to object to NULL
     */
    void detach()
    {
       if (m_cached_object)
       {
            m_cached_object->delRef();
       }
       m_changed = true;
       m_cached_object = NULL;
       m_link.detach();
    }
    /*! Sets database for a link
        \param[in] db a linked database
     */
    void setDatabase(sad::db::Database * db)
    {
        m_link.setDatabase(db);
        m_changed = true;
    }
    /*! Sets path to an abstract link
        \param[in] path path to a linked resource
    */
    void setPath(sad::String path)
    {
        m_link.setPath(path);
        m_changed = true;
    }
    /*! Returns path to a linked resource
        \return path to a linked resource
    */
    const sad::String & path() const
    {
        return m_link.path();
    }
    /*! Returns linked tree's name
        \return name of tree
    */
    const sad::String & treeName() const
    {
        return m_link.treeName();
    }
    /*! Sets a tree for a link
        \param[in] tree a tree for link
    */
    void setTree(sad::resource::Tree * tree)
    {
        m_link.setTree(tree);
        m_changed = true;
    }
    /*! Returns tree, which link references to
        \return tree for a link
    */
    sad::resource::Tree* tree() const
    {
        return m_link.tree();
    }
    /*! Sets a tree for a link, defined by renderer, where it's stored
        and treename
        \param[in] r renderer
        \param[in] treename a trename
    */
    void setTree(sad::Renderer * r, const sad::String& treename = "")
    {
        m_link.setTree(r, treename);
        m_changed = true;
    }
    /*! Whether link depends on renderer
        \return whether link depends on renderer
    */
    bool dependsOnRenderer() const
    {
        return m_link.dependsOnRenderer();
    }
    /*! Returns renderer, which link depends upon
        \return renderer
    */
    sad::Renderer * renderer() const
    {
        return m_link.renderer();
    }
    /*! Sets new renderer for a link
        \param[in] r renderer a renderer
    */
    void setRenderer(sad::Renderer * r)
    {
        m_link.setRenderer(r);
        m_changed = true;
    }
    /*! Returns an object, referenced by link
        \return object
     */
    T* get() const
    {
        sad::resource::StrongLink<T>* me = const_cast<sad::resource::StrongLink<T> *>(this);
        if (!m_changed)
        {
            return m_cached_object;
        }
        T* result = NULL;
        sad::resource::Resource* o = me->m_link.resource();
        if (o)
        {
            sad::db::TypeName<T>::init();
            if (o->isInstanceOf(sad::db::TypeName<T>::name()))
            {
                result = static_cast<T*>(o);
            }
        }
        if (me->m_cached_object)
        {
            me->m_cached_object->delRef();
        }
        me->m_cached_object = result;
        if (me->m_cached_object)
        {
            me->m_cached_object->addRef();
            me->m_changed = false;
        }
        return me->m_cached_object;
    }
protected:
    /*! A cached object
     */
    T* m_cached_object;
    /*! Whether innert value for strong link is changed
     */
    bool m_changed;
    /*! An inner link for objects
     */ 
    sad::resource::Link<T> m_link;
};

}

}
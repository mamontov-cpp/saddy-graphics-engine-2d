/*! \file treedblink.h
    \author HiddenSeeker

    Defines a link, which could either point to tree or to database
 */
#pragma once
#include "db/dbobject.h"
#include "db/dblink.h"
#include "resource/tree.h"
#include "resource/link.h"

namespace sad
{

/*! A link, which could could either point to database or to
    tree resource
 */
template<
    typename T
>
class TreeDbLink
{
public:
    /*! Constructs new empty link
     */
    TreeDbLink() : m_referenced(false), m_point_to_resource(false)
    {

    }
    /*! Copies link from other tree
        \param[in] o other link
     */
    TreeDbLink(const sad::TreeDbLink<T>& o)
    : m_referenced(o.m_referenced),
    m_resource(o.m_resource),
    m_db(o.m_db),
    m_point_to_resource(o.m_point_to_resource)
    {
        if (m_referenced)
        {
            object(false)->addRef();
        }
    }
    /*! Assigns an object to a link
        \param[in] o object other link
        \return *this
     */
    sad::TreeDbLink<T>& operator=(const sad::TreeDbLink<T>& o)
    {
        if (m_referenced)
        {
            object(false)->delRef();
        }
        m_referenced = o.m_referenced;
        m_resource = o.m_resource;
        m_db = o.m_db;
        m_point_to_resource = o.m_point_to_resource;
        if (m_referenced)
        {
            object(false)->addRef();
        }
        return *this;
    }
    /*! Frees reference on resource
     */
    ~TreeDbLink()
    {
        if (m_referenced)
        {
            object(false)->delRef();
        }
    }
    /*! Sets tree for a name
        \param[in] r renderer
        \param[in] treename a name for tree
     */
    void setTree(sad::Renderer * r, const sad::String& treename = "")
    {
        if (m_referenced)
        {
            object(false)->delRef();
        }
        m_resource.setTree(r, treename);
        m_point_to_resource = true;
        m_referenced = false;
    }

	/*! Sets tree for a name
        \param[in] tree a tree
     */
	void setTree(sad::resource::Tree * tree)
    {
        if (m_referenced)
        {
            object(false)->delRef();
        }
        m_resource.setTree(tree);
        m_point_to_resource = true;
        m_referenced = false;
    }
    /*! Sets database for a tree database
        \param[in] db database
     */
    void setDatabase(sad::db::Database* db)
    {
        if (m_referenced)
        {
            object(false)->delRef();
        }
        m_db.setDatabase(db);
        m_point_to_resource = false;
        m_referenced = false;
    }
    /*! Sets an object for link
        \param[in] object an object for tree database
     */
    void setObject(T* object)
    {
        if (m_referenced)
        {
            this->object(false)->delRef();
        }
        object->addRef();
        m_referenced = true;
        m_db.setObject(object);
        m_point_to_resource = false;
    }
    /*! Returns an object
        \param[in] check  whether typecheck should be performed
        \return object (NULL if not found)
     */
    T* object(bool check = true) const
    {
        sad::TreeDbLink<T>* me = const_cast<sad::TreeDbLink<T>*>(this);
        T* result = NULL;
        if (m_point_to_resource)
        {
            result = m_resource.get();
        }
        else
        {
            sad::db::Object* o = me->m_db.get();
            if (check)
            {
                sad::db::TypeName<T>::init();
                // Base name is intended, nobody shouldn't use it
                // on pointers
                if (o->isInstanceOf(sad::db::TypeName<T>::baseName()))
                {
                    result  = static_cast<T*>(o);
                }
            }
            else
            {
                result  = static_cast<T*>(o);
            }
        }
        if (result && m_referenced == false)
        {
            result->addRef();
            me->m_referenced = true;
        }

        return result;
    }
    /*! Sets name for resource, linking it to resource tree
        \param[in] name name for resource
     */
    void setName(const sad::String& name)
    {
        if (m_referenced)
        {
            object(false)->delRef();
        }
        m_resource.setPath(name);
        m_point_to_resource = true;
        m_referenced = false;
    }
    /*! Returns name for a resource, if linked to tree. Empty string
        otherwise
        \return name for a resource, if linked to tree.
     */
    sad::String name() const
    {
        if (m_point_to_resource)
        {
            return m_resource.path();
        }
        return "";
    }
    /*! Sets major id for link
        \param[in] id a major id for link
     */
    void setMajorId(unsigned long long id)
    {
        if (m_referenced)
        {
            object(false)->delRef();
        }
        m_db.setMajorId(id);
        m_point_to_resource = false;
        m_referenced = false;
    }
    /*! Returns major id for resource if linked to database, 0 otherwise
        \return major id
     */
    unsigned long long majorId() const
    {
        if (m_point_to_resource == false)
        {
            return m_db.majorId();
        }
        return 0;
    }
protected:
    /*! Whether we point to resource
     */
    bool m_point_to_resource;
    /*! Whether this link is referenced
     */
    bool m_referenced;
    /*! A database link to database
     */
    sad::db::Link m_db;
    /*! A resource link for tree
     */
    sad::resource::Link<T> m_resource;
};

}

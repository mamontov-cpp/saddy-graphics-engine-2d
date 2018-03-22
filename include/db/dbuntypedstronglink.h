/*! \file db/dbuntypedstronglink.h
    
    
    Describes a strong link to untyped database object, used to link object from a database, storing a reference to it.
 */ 
#pragma once
#include "../sadstring.h"
#include "dbdatabase.h"
#include "dblink.h"

namespace sad
{

namespace db
{
/*! A strong link to object, with reference counting
 */
class UntypedStrongLink
{
public:
    /*! Constructs new strong link for database objects
     */
    inline UntypedStrongLink() : m_cached_object(NULL), m_changed(true)
    {
        
    }
    /*! Constructs new link via copying
        \param[in] o other object
     */ 
    inline UntypedStrongLink(const sad::db::UntypedStrongLink& o) : m_cached_object(NULL), m_changed(true), m_link(o.m_link)
    {
        
    }
    /*! Copies an inner state for object from source
        \param[in] o object
        \return self-reference
     */
    inline sad::db::UntypedStrongLink& operator=(const sad::db::UntypedStrongLink& o)
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
    inline ~UntypedStrongLink()
    {
        if (m_cached_object)
        {
            m_cached_object->delRef();
        }
    }
    /*! Sets database for a link
        \param[in] db a linked database
     */
    inline void setDatabase(sad::db::Database * db)
    {
        m_link.setDatabase(db);
        m_changed = true;
    }
    /*! Sets a table name for a  link
        \param[in] t a table name
     */
    inline void setTableName(const sad::String & t)
    {
        m_link.setTableName(t);
        m_changed = true;
    }
    /*! Sets linked table for a link
        \param[in] t a table for a link
     */
    inline void setTable(sad::db::Table * t)
    {
        m_link.setTable(t);
        m_changed = true;
    }
    /*! Makes link fetch object by it's major id
        \param[in] major_id a specified major id, which object must have
     */
    inline void setMajorId(unsigned long long major_id)
    {
        m_link.setMajorId(major_id);
        m_changed = true;
    }
    /*! A major id for a link
        \return major id or 0, if objects are linked by other ways
     */
    inline unsigned long long majorId() const
    {
        return m_link.majorId();
    }
    /*! Makes link fetch first object by it's name
        \param[in] name a specified name of object
     */
    inline void setName(const sad::String & name)
    {
        m_link.setName(name);
        m_changed = true;
    }
    /*! Returns name of linked object, it it must be fetched by it or empty string otherwise
        \return name of object or empty string
     */ 
    inline const sad::String & name() const
    {
        return m_link.name();
    }
    /*! Explicitly sets cached object
        \param[in] o new object
     */
    inline void setObject(sad::db::Object* o)
    {
        if (m_cached_object)
        {
            m_cached_object->delRef();
        }
        m_link.setObject(o);
        m_cached_object = o;
        if (o)
        {
            o->addRef();
        }
        m_changed = false;
    }
    /*! Returns an object, referenced by link
        \return object
     */
    inline sad::db::Object* get() const
    {
        sad::db::UntypedStrongLink* me = const_cast<sad::db::UntypedStrongLink *>(this);
        if (!m_changed)
        {
            return m_cached_object;
        }
        sad::db::Object* result = NULL;
        sad::db::Object* o = me->m_link.get();
        if (o)
        {
            result = o;
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
    sad::db::Object* m_cached_object;
    /*! Whether innert value for strong link is changed
     */
    bool m_changed;
    /*! An inner link for objects
     */ 
    sad::db::Link m_link;
};

}

}
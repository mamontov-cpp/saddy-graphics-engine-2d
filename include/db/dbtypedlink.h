/*! \file dbtypedlink.h
    

    Defines a typed link to database
 */
#pragma once
#include "dbtypename.h"
#include "dblink.h"
#include "dbobject.h"

namespace sad
{
    
namespace db
{
    
template<
    typename T
>
class TypedLink: public sad::db::Link
{
public:    
    /*! Creates new typed link
     */
    TypedLink() : m_old_resource_value(nullptr), m_cached_link_value(nullptr)
    {
        
    }
    /*! Can be inherited
     */
    virtual ~TypedLink()
    {
        
    }
    /*! Like get() but typed
        \return value (nullptr if not found or incompatible types)
     */
    T* value()
    {
        sad::db::Object* o = get();
        if (m_old_resource_value != o)
        {
            m_old_resource_value = o;
            m_cached_link_value = nullptr;
            if (o)
            {
                sad::db::TypeName<T>::init();
                if (o->isInstanceOf(sad::db::TypeName<T>::name()))
                {
                    m_cached_link_value = static_cast<T*>(o);   
                }
            }            
        }
        return m_cached_link_value;
    }
protected:
    /*! Old resource value for typed link
     */
    sad::db::Object* m_old_resource_value;
    /*! A cached link value
     */
    T* m_cached_link_value;
};

}

}
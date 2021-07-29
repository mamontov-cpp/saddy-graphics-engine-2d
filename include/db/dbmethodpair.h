/*! \file   dbmethodpair.h
    
    
    Describes a pair of methods, which could be used to work as property
 */
#pragma once
#include "dbproperty.h"
#include "../util/getterproxy.h"
#include "../util/setterproxy.h"

#include <cassert>

namespace sad
{

namespace db
{

/*! Describes a pair of methods, which could be used to get and set data
 */
template<
    typename _Object,
    typename _FieldTypeName
>
class MethodPair: public sad::db::Property
{
public:
    /*! Setups a pair of methods
        \param[in] g getter part
        \param[in] s setter part
     */
    inline MethodPair(
        sad::util::getter::Proxy<_Object, _FieldTypeName> * g, 
        sad::util::setter::Proxy<_Object, _FieldTypeName> * s
    ) : m_getter(g), m_setter(s)
    {
        sad::db::TypeName<_FieldTypeName>::init();
        m_base_type = sad::db::TypeName<_FieldTypeName>::baseName();
        m_type_is_kind_of_sad_object = sad::db::TypeName<_FieldTypeName>::isSadObject();
        m_pointer_stars_count = sad::db::TypeName<_FieldTypeName>::POINTER_STARS_COUNT;
    }
    /*! Sets a pair of methods
        \param[in] g getter part
        \param[in] s setter part
     */
    template<
        typename _Getter,
        typename _Setter
    >
    inline MethodPair(
        _Getter g, 
        _Setter s
    ) 
    {
        m_getter = sad::util::define_getter<_Object, _FieldTypeName>(g);
        m_setter = sad::util::define_setter<_Object, _FieldTypeName>(s);
        sad::db::TypeName<_FieldTypeName>::init();
        m_base_type = sad::db::TypeName<_FieldTypeName>::baseName();
        m_type_is_kind_of_sad_object = sad::db::TypeName<_FieldTypeName>::isSadObject();
        m_pointer_stars_count = sad::db::TypeName<_FieldTypeName>::POINTER_STARS_COUNT;
    }
    /*! Frees all proxies
     */
    virtual ~MethodPair() override
    {
        delete m_getter;
        delete m_setter;
    }

    /*! Clones a property
        \return a property clone
     */
    virtual sad::db::Property* clone() const override
    {
	    auto* result = new sad::db::MethodPair<_Object,_FieldTypeName>(m_getter->clone(), m_setter->clone());
        if (m_default_value)
        {
            result->m_default_value = new sad::db::Variant(*m_default_value);
        }
        return result;
    }
    /*! Sets a value for a property
        \param[in] o an object
        \param[in] v a value for property
        \return whether value is set successfully
     */
    virtual bool set(sad::db::Object * o, const sad::db::Variant & v) override
    {
        assert( o );
        sad::Maybe<_FieldTypeName> value = v.get<_FieldTypeName>();
        bool result = false;
        if (value.exists() && o)
        {
            m_setter->set(reinterpret_cast<_Object*>(o), value.value());
            result = true;
        }
        return result;
    }
    /*! Gets a value for a property
        \param[in] o an object
        \param[in] v a value for a property
     */
    virtual void get(sad::db::Object const* o, sad::db::Variant & v) const override
    {
        assert( o );
        if (o)
        {
            v.set(m_getter->get(reinterpret_cast<_Object const*>(o)));
        }
    }

    /*! Checks, whether value has property type in key field
        \param[in] key a key of field to check
        \param[in] v value
        \return whether field has following type
     */
    virtual bool check(const sad::String& key, const picojson::value& v) override
    {
        bool result = false;
        if (v.is<picojson::object>())
        {
	        const picojson::value& data = v.get(key);
            if (data.is<picojson::null>() == false) 
            {
                result = picojson::ValueToType<_FieldTypeName>().get(data).exists();
            }
        }
        return result;
    }
    /*! Returns true, if property value could be set from specified value
        \param[in] v value
        \return whether it could be set from value
     */ 
    virtual bool couldBeSetFrom(const sad::db::Variant& v) override
    {
        return v.get<_FieldTypeName>().exists();
    }
protected:
    /*! A temporary variant for returning property from field
     */
    sad::db::Variant m_tmp;
    /*! A getter for method pair
     */
    sad::util::getter::Proxy<_Object, _FieldTypeName> * m_getter;
    /*! A setter for method pair
     */
    sad::util::setter::Proxy<_Object, _FieldTypeName> * m_setter;
};

}

}

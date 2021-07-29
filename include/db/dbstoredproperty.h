/*! \file   dbfield.h
    
    
    Describes a field of class, as property
 */
#pragma once
#include "dbproperty.h"
#include "dbvariant.h"
#include <cassert>
#include <utility>

namespace sad
{

namespace db
{

/*! Describes a mapped field for a class
 */
template<
    typename _FieldTypeName
>
class StoredProperty: public sad::db::Property
{
public:
    /*! Creates new field for a class
        \param[in] v a field data
     */
    StoredProperty(_FieldTypeName v = _FieldTypeName()) : m_value(std::move(v))
    {
        sad::db::TypeName<_FieldTypeName>::init();
        m_base_type = sad::db::TypeName<_FieldTypeName>::baseName();
        m_type_is_kind_of_sad_object = sad::db::TypeName<_FieldTypeName>::isSadObject();
        m_pointer_stars_count = sad::db::TypeName<_FieldTypeName>::POINTER_STARS_COUNT;
    }
    /*! A field data
     */
	virtual ~StoredProperty()  override = default;
    /*! Clones a property
        \return a property clone
     */
    virtual sad::db::Property* clone() const override
    {
        sad::db::StoredProperty<_FieldTypeName>* result = new sad::db::StoredProperty<_FieldTypeName>(m_value);
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
        sad::Maybe<_FieldTypeName> value = v.get<_FieldTypeName>();
        bool result = false;
        if (value.exists())
        {
            m_value  = value.value();
            result = true;
        }
        return result;
    }
    /*! Gets a value for a property
        \param[in] o an object
        \param[in] v a value for a property
     */
    virtual void  get(sad::db::Object const* o, sad::db::Variant & v) const  override
    {
        v.set(m_value);
    }
    /*! Checks, whether value has property type in key field
        \param[in] key a key of field to check
        \param[in] v value
        \return whether field has following type
     */
    virtual bool check(const sad::String& key, const picojson::value& v)  override
    {
        bool result = false;
        if (v.is<picojson::object>())
        {
            picojson::value data = v.get(key);
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
    /*! A value for field of class
     */
    _FieldTypeName m_value;
};

}

}

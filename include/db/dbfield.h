/*! \file   dbfield.h
    
    
    Describes a field of class, as property
 */
#pragma once
#include "dbproperty.h"
#include "dbvariant.h"
#include <cassert>

namespace sad
{

namespace db
{

/*! Describes a mapped field for a class
 */
template<
    typename _Object,
    typename _FieldTypeName
>
class Field: public sad::db::Property
{
public:
    /*! Creates new field for a class
        \param[in] f a field data
     */
    Field(_FieldTypeName _Object::*f) : sad::db::Property(), m_f(f)
    {
        sad::db::TypeName<_FieldTypeName>::init();
        m_base_type = sad::db::TypeName<_FieldTypeName>::baseName();
        m_type_is_kind_of_sad_object = sad::db::TypeName<_FieldTypeName>::isSadObject();
        m_pointer_stars_count = sad::db::TypeName<_FieldTypeName>::POINTER_STARS_COUNT;
    }
    /*! A field data
     */
    virtual ~Field()
    {

    }
    /*! Clones a property
        \return a property clone
     */
    virtual sad::db::Property* clone() const override
    {
        sad::db::Field<_Object,_FieldTypeName>* result = new sad::db::Field<_Object,_FieldTypeName>(m_f);
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
            (reinterpret_cast<_Object*>(o)->*m_f)  = value.value();
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
        if (o)
        {
            v.set(reinterpret_cast<_Object const*>(o)->*m_f);
        }
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
    /*! A field of class
     */
    _FieldTypeName _Object::*m_f;
};

/*! Returns a new field by value
    \param[in] f object
    \return field
 */
template<
    typename _Object,
    typename _FieldTypeName
>
Field<_Object, _FieldTypeName> * define_field(_FieldTypeName _Object::*f)
{
    return new Field<_Object, _FieldTypeName>(f);
}

}

}

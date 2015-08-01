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
		\param[in] o a field data
	 */
	Field(_FieldTypeName (_Object::*f)) : sad::db::Property(), m_f(f)
	{
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
	virtual sad::db::Property* clone() const
	{
		return new sad::db::Field<_Object,_FieldTypeName>(m_f);
	}
	/*! Sets a value for a property
		\param[in] o an object
		\param[in] v a value for property
		\return whether value is set successfully
	 */
	virtual bool set(sad::db::Object * o, const sad::db::Variant & v)
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
	virtual void  get(sad::db::Object const* o, sad::db::Variant & v) const 
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
	virtual bool check(const sad::String& key, const picojson::value& v) 
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

protected:
	/*! A field of class
	 */
	_FieldTypeName (_Object::*m_f);
};

/*! Returns a new field by valye
	\param[in] f object
	\return field
 */
template<
	typename _Object,
	typename _FieldTypeName
>
Field<_Object, _FieldTypeName> * define_field(_FieldTypeName (_Object::*f))
{
	return new Field<_Object, _FieldTypeName>(f);
}

}

}

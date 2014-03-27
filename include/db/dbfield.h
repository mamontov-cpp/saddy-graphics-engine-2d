/*! \file   dbfield.h
	\author HiddenSeeker
	
	Describes a field of class, as property
 */
#pragma once
#include "dbproperty.h"
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
	Field(_FieldTypeName (_Object::*f)) : m_f(f)
	{
		
	}
	/*! A field data
	 */
	virtual ~Field()
	{

	}
	/*! Sets a value for a property
		\param[in] v a variant for property
		\return whether value is set successfully
	 */
	virtual bool setValue(const sad::db::Variant & v)
	{
		assert( m_o );
		sad::Maybe<_FieldTypeName> value = v.get<_FieldTypeName>();
		bool result = false;
		if (value.exists())
		{
			(static_cast<_Object*>(m_o)->*m_f)  = value.value();
			result = true;
		}
		return result;
	}
	/*! Gets a value for a property
		\return a value for a property
	 */
	virtual const sad::db::Variant & getValue() const
	{
		assert( m_o );
		m_tmp.set( (static_cast<_Object*>(m_o)->*m_f) );
		return m_tmp;
	}

protected:
	/*! A temporary variant for returning property from field
	 */
	sad::db::Variant m_tmp;
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

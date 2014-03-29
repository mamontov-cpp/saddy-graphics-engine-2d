/*! \file   dbmethodpair.h
	\author HiddenSeeker
	
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
			m_setter->set(reinterpret_cast<_Object*>(m_o), value.value());
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
		_FieldTypeName v = m_getter->get(reinterpret_cast<_Object*>(m_o));
		const_cast<sad::db::Variant&>(m_tmp).set( v );
		return m_tmp;
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

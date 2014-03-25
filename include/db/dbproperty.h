/*! \file dbproperty.h
	\author HiddenSeeker

	Describes a basic property, which objects are consists of
 */
#pragma once
#include "dbvariant.h"

namespace sad
{

namespace db
{

/*! A basic property, which objects are consists of
 */
class Property
{
public:
	/*! Constructs new property
	 */
	Property();
	/*! Can be inherited
	 */
	virtual ~Property();
	/*! Sets object for a property
		\param[in] o object
	 */
	virtual void setObject(sad::Object * o) 
	{
		m_o = o;
	}
	/*! Sets a value for a property
		\param[in] v a variant for property
		\return whether value is set successfully
	 */
	virtual bool setValue(const sad::db::Variant & v) = 0;
	/*! Gets a value for a property
		\return a value for a property
	 */
	virtual const sad::db::Variant & getValue() const = 0;
	/*! Sets a value for a property
		\param[in] v a new value of property
		\return whether value is set successfully
	 */
	template<
		typename T
	>
	bool set(const T & v)
	{
		return setValue(sad::db::Variant(v));
	}
	/*! Returns a value for property
		\return value for a property
	 */
	template<
		typename T
	>
	sad::Maybe<T> get() const
	{
		return getValue().get<T>();
	}
protected:
	/*! Object, which property belongs to
	 */
	sad::Object * m_o;
};

}

}

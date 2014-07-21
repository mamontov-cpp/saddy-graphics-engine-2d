/*! \file dbproperty.h
	\author HiddenSeeker

	Describes a basic property, which objects are consists of
 */
#pragma once
#include "dbvariant.h"
#include "../sadstring.h"
#include "../3rdparty/picojson/valuetotype.h"

namespace sad
{

namespace db
{
class Object;

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
	/*! Returns name of type of property
		\return type of property
	 */
	virtual const sad::String & type() const;
	/*! Returns whether type of property is kind of sad::Object
		\return whether type of property is kind of sadd::Object
	 */
	virtual bool typeIsKindOfSadObject() const;
	/*! Sets a value for a property
		\param[in] o an object
		\param[in] v a variant for property
		\return whether value is set successfully
	 */
	virtual bool set(sad::db::Object * o, const sad::db::Variant & v) = 0;
	/*! Gets a value for a property
		\param[in] o an object
		\param[in] v a value for a property
	 */
	virtual void get(sad::db::Object * o, sad::db::Variant & v) const = 0;
	/*! Sets a value for a property
		\param[in] o object
		\param[in] v a new value of property
		\return whether value is set successfully
	 */
	template<
		typename T
	>
	bool set(sad::db::Object* o, const T & v)
	{
		return set(o, sad::db::Variant(v));
	}
	/*! Returns a value for property
		\param[in] o object
		\return value for a property
	 */
	template<
		typename T
	>
	sad::Maybe<T> get(sad::db::Object * o) const
	{
		sad::db::Variant v;
		this->get(o, v);
		return v.get<T>();
	}

	/*! Checks, whether value has property type in key field
		\param[in] key a key of field to check
		\param[in] v value
		\return whether field has following type
	 */
	virtual bool check(const sad::String& key, const picojson::value& v);

protected:
	/*! Amount of pointers for type
	 */
	int m_pointer_stars_count;
	/*! True if type of property is kind of sad object
	 */
	bool m_type_is_kind_of_sad_object;
	/*! A base name of type of property (a name of type without a pointers)
	 */
	sad::String m_base_type;
};

}

}

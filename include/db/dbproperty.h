/*! \file dbproperty.h
	\author HiddenSeeker

	Describes a basic property, which objects are consists of
 */
#pragma once
#include "../sadstring.h"
#include "dbtypename.h"
#include "../3rdparty/picojson/valuetotype.h"

namespace sad
{

namespace db
{
class Object;
class Variant;

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
	virtual const sad::String & baseType() const;
	/*! Returns whether type of property is kind of sad::Object
		\return whether type of property is kind of sadd::Object
	 */
	virtual bool typeIsKindOfSadObject() const;
	/*! Returns count for pointer stars count
		\return pointer stars count data
	 */
	virtual int pointerStarsCount() const;
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
	virtual void get(sad::db::Object const* o, sad::db::Variant & v) const = 0;
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

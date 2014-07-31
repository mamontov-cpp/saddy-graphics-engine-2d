/*! \file db/object.h
	\author HiddenSeeker
	
	Contains definition of basic serializable class Object.
 */
#pragma once
#include "../3rdparty/picojson/valuetotype.h"
#include "dbproperty.h"
#include "dbcanbecastedfromto.h"
#include "dbvariant.h"

namespace sad
{

namespace db
{

namespace schema
{
class Schema;
}

class Property;
class Table;
/*! \class Object
	
	Defines a basic serializable object
 */
class Object  
{	
public:	
	/*! A default object constructor
	 */
	Object();
	/*! This class can be inherited 
	 */
	virtual ~Object();
	/*! Saves object to a picojson::value
		\return	a value with saved properties
	 */
	virtual picojson::value save();
	/*! Loads an object to a value
		\param[in] v a value for object
		\return result value
	 */
	virtual bool load(const picojson::value& v);
	/*! Returns a table, where object belongs
		\return table
	 */
	virtual sad::db::Table* table() const;
	/*! Sets a table
		\param[in] t a table
	 */
	virtual void setTable(sad::db::Table* t);
	/*! Returns schema for an object
		\return schema
	 */
	virtual sad::db::schema::Schema* schema() const;
	/*! A name, under which object could be serialized
		\return name, under which object will be serialized
	 */
	virtual const sad::String& serializableName() const;
	/*! Tries to fetch property for  an object
		\param[in] s name of property
		\return property name
	 */
	template<
		typename T
	>
	sad::Maybe<T> getProperty(const sad::String & s) const
	{
		sad::db::Property * prop = this->getObjectProperty(s);
		sad::Maybe<T> result;
		if (prop && this)
		{
			bool canbecasted = sad::db::can_be_casted_from_to(
				prop->baseType(), 
				prop->typeIsKindOfSadObject(),
				prop->pointerStarsCount(),
				sad::db::TypeName<T>::baseName(),
				sad::db::TypeName<T>::isSadObject(),
				sad::db::TypeName<T>::POINTER_STARS_COUNT
			);
			if (canbecasted)
			{
				sad::db::Variant v;
				prop->get(this, v);
				result = v.get<T>();
			}
		}
		return result;
	}
	/*! Sets a property for an object
		\param[in] s a name of property
		\param[in] o value for property
		\return whether property is successfully set
	 */ 
	template<
		typename T
	>
	bool setProperty(const sad::String & s, const T & o)
	{
		sad::db::Property * prop = this->getObjectProperty(s);
		bool result = false;
		if (prop && this)
		{
			bool canbecasted = sad::db::can_be_casted_from_to(
				sad::db::TypeName<T>::baseName(),
				sad::db::TypeName<T>::isSadObject(),
				sad::db::TypeName<T>::POINTER_STARS_COUNT,
				prop->baseType(), 
				prop->typeIsKindOfSadObject(),
				prop->pointerStarsCount()
			);
			if (canbecasted)
			{
				sad::db::Variant v(o);
				result = prop->set(this, v);
			}
		}
		return result;
	}
	/*! A major id for object
	 */
	int MajorId;
	/*! A minor id for a table
	 */
	int MinorId;
protected:
	/*! Fetches property for an object with specified game
		\param[in] s string
		\return s string
	 */
	sad::db::Property* getObjectProperty(const sad::String& s) const;
	/*! A table for object, null by default
	 */
	sad::db::Table* m_table;
};

}

}

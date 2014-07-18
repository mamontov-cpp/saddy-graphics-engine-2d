/*! \file db/object.h
	\author HiddenSeeker
	
	Contains definition of basic serializable class Object.
 */
#pragma once
#include "../3rdparty/picojson/valuetotype.h"

namespace sad
{

namespace db
{

namespace schema
{
class Schema;
}

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
	picojson::value save();
	/*! Loads an object to a value
		\param[in] v a value for object
		\return result value
	 */
	bool load(const picojson::value& v);
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
	virtual sad::db::schema::Schema * schema() const;
	/*! A major id for object
	 */
	int MajorId;
	/*! A minor id for a table
	 */
	int MinorId;
protected:
	/*! A table for object, null by default
	 */
	sad::db::Table* m_table;
};

}

}

/*! \file db/database.h
	\author HiddenSeeker
	
	Contains definition of class Database, which is a database of game objects, which could be organized in tables
	and queried by various parameters, like name and identifying ids.
 */
#pragma once
#include "../sadstring.h"
#include "dbobjectfactory.h"
#include "dberror.h"
#include "dbtable.h"
#include "dbstoredpropertyfactory.h"
#include "dbstoredproperty.h"
#include "dbcanbecastedfromto.h"
#include "dbvariant.h"

namespace sad
{

class Renderer;

namespace db
{

/*! \class Database

	A database of game objects, which could be organized in tables
	and queried by various parameters, like name and identifying major and minor ids.
 */
class Database  
{	
public:	
	/*! Constructs default empty database
	 */
	Database();
	/*! This class can be inherited 
	 */
	virtual ~Database();
	/*! Saves database to string
		\param[out] output output string
	 */
	void save(sad::String & output);
	/*! Saves database to file
		\param[in] filename a name of file
	 */
	void saveToFile(const sad::String& filename);
	/*! Loads database from specified string
		\param[in] text a text with JSON description of database
		\return whether load was successfull
	 */
	bool load(const sad::String& input);
	/*! Loads database from file, using specifying name
		\param[in] name a name for file
		\param[in] r renderer, which is used to determine global path's (NULL for global)
		\return whether load was successfull
	 */
	bool loadFromFile(const sad::String& name, sad::Renderer * r = NULL);
	/*! Adds new custom property to database. Replaces another property, if such property exists.
		\param[in] name name of property
		\param[in] p a property
	 */
	virtual void addProperty(const sad::String & name, sad::db::Property * p);
	/*! Removes a custom property from data. A custom property is freed and excluded from memory
		\param[in] name name of property
	 */
	virtual void removeProperty(const sad::String & name);
	/*! Returns a property by it's name
		\param[in] name a name of property
		\return value of property
	 */
	virtual sad::db::Property* propertyByName(const sad::String & name) const;
	/*! Tries to set property to a value 
		\param[in] name a name of property
		\param[in] value a value of property
		\return whether setting property was successfull
	 */
	template<
		typename T
	>
	bool setProperty(const sad::String & name, const T & value)
	{
		sad::db::Property * prop = this->propertyByName(name);
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
				sad::db::Variant v(value);
				result = prop->set(NULL, v);
			}
		}
		return result;
	}
	/*! Tries to fetch property value from a database
		\param[in] s name of property
		\return property value
	 */
	template<
		typename T
	>
	sad::Maybe<T> getProperty(const sad::String & s) const
	{
		sad::db::Property * prop = this->propertyByName(s);
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
				prop->get(NULL, v);
				result = v.get<T>();
			}
		}
		return result;
	}
	/*! Returns new stored property factory for a database
		\return stored property factory in database
	 */
	sad::db::StoredPropertyFactory* storedPropertyFactory() const;
	/*! Sets a stored property factory for a database. Old factory will
		be deleted.
		\param[in] f a factory.
	 */
	void setStoredPropertyFactory(sad::db::StoredPropertyFactory * f);
	/*! Adds new table to database. If table already exists, you can't replace it
		\param[in] name a name for table
		\param[in] table a table name
		\return whether table was added to databases
	 */
	bool addTable(const sad::String& name, sad::db::Table* table);
	/*! Removes table from database by name. All objects from it will be freed afterwards.
		\param[in] name a name for table
	 */
	void removeTable(const sad::String& name);
	/*! Returns table by it's name 
		\param[in] name a name for a table
		\return a name for a table
	*/
	sad::db::Table* table(const sad::String& name);
	/*! Returns new unique major id. Note, that new id is being registered in db for specified table
		\param[in] t a table
		\return new unique major id
	 */
	unsigned long long uniqueMajorId(sad::db::Table * t);
	/*! Queries all tables in search of object by name
		\param[in] name name of searched objects
		\return a vector of objects by name
	 */
	sad::Vector<sad::db::Object *> queryByName(const sad::String & name) const;
	/*! Queries all tables in seatch of object by minor id
		\param[in] id a minor id of searched objects
		\return a vector of objects by name
	 */
	sad::Vector<sad::db::Object *> queryByMinorId(unsigned long long id) const;
	/*! Queries tables by major id
		\param[in] id  a major if of searched objects
		\return object
	 */
	sad::db::Object * queryByMajorId(unsigned long long id) const;
	/*! Fetches tables from a database
		\param[out] tables a tables in database
	 */
	void getTables(sad::Vector<sad::Pair<sad::String, sad::db::Table*> > & tables) const;
	/*! Returns object factory for a database
		\return  object factory for a database
	 */
	sad::db::ObjectFactory* factory();
	/*! Sets object factory for a database
		\param[in] f object factory for a database
	 */
	void setFactory(sad::db::ObjectFactory* f);
	/*! Tries to set new max major id in database 
		\param[in] v a value
		\param[in] t a table, which want to set it
	 */
	void trySetMaxMajorId(unsigned long long v, sad::db::Table * t);
	/*! Removes major id from hint tables
		\param[in] v a major id
	 */
	void removeMajorId(unsigned long long v);
	/*! Sets renderer for database. Database will use this renderer to take executable path from it
		\param[in] r renderer
	 */
	void setRenderer(sad::Renderer * r);
	/*! Returns local renderer, if set, otherwise returns global renderer
	 */
	sad::Renderer* renderer() const;
protected: 
	/*! Loads properties and tavles from a database
		\param[in] properties a property items
		\param[in] tables a tables from database
		\return whether it was successfull
	 */
	bool loadPropertiesAndTables(
		const picojson::object & properties, 
		const picojson::object & tables
	);
	/*! Current maximal major id in database. Every object should have major id less,
		than it.
	 */
	unsigned long long m_max_major_id;
	/*! A links from major id to specified table
	 */
	sad::Hash<unsigned long long, sad::db::Table*> m_majorid_to_table;
	/*! A names of tables, linked to tables
	 */
	sad::Hash<sad::String, sad::db::Table*> m_names_to_tables;
	/*! A factory of objects
	 */
	sad::db::ObjectFactory* m_factory;
	/*! A stored property factory, used to link objects to properties
	 */
	sad::db::StoredPropertyFactory* m_prop_factory;
	/*! A map of properties,linked to database
	 */
	sad::PtrHash<sad::String, sad::db::Property> m_properties;
	/*! Linked renderer in database
	 */
	sad::Renderer * m_renderer;
};

}

}


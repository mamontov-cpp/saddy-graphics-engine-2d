/*! \file db/dbtable.h
	\author HiddenSeeker
	
	Contains definition of class Table, which represents object table in database.
 */
#pragma once
#include "dbobject.h"

namespace sad
{

namespace db
{

class Database;
class ObjectFactory;
/*! \class Table

	A table, which represents objects in database, which could be used to fetch objects
	dynamically and do some other stuff.
 */
class Table  
{	
public:	
	/*! Creates new empty table
	 */
	Table();
	/*! This class can be inherited 
	 */
	virtual ~Table();
	/*! Adds new object to a table
		\param[in] a an object
	 */
	virtual void add(sad::db::Object* a);
	/*! Removes objects from table
		\param[in] a an object
	 */
	virtual void remove(sad::db::Object* a);
	/*! Queries a table by full key
		\param[in] major_id a major id, stored in DB
		\param[in] minor_id a minor local key
		\return a found object (NULL if not found) 
	 */
	virtual sad::db::Object* queryById(unsigned long long major_id, unsigned long long minor_id);
	/*! Query table by minor id
		\param[in] minor id a minor id
		\return a found object (NULL if not found)
	 */
	virtual sad::db::Object* queryByMinorId(unsigned long long minor_id);
	/*! Queries a table by name
		\param[in] name a name
		\return a vector of linked object
	 */
	virtual sad::Vector<sad::db::Object*> queryByName(const sad::String& name);
	/*! Queries objects from table by major id field
		\param[in] major_id an id
		\return	an object, or null if not found
	 */
	virtual sad::db::Object* queryByMajorId(unsigned long long major_id);
	/*! Loads table from a  value
		\param[in] v value
		\param[in] factory a factory
		\return whether value was successfull
	 */
	virtual bool load(const picojson::value & v, sad::db::ObjectFactory * factory);
	/*! Saves a table to a value
		\param[out] v a value for table
	 */
	virtual void save(picojson::value & v);
	/*! Returns database, linked with table
		\return database
	 */
	sad::db::Database* database() const;
	/*! Sets database for a table
		\param[in] d database
	 */
	void setDatabase(sad::db::Database* d);
	/*! Fills vector with objects from table
		\param[out] o objects
	 */
	virtual void objects(sad::Vector<sad::db::Object*> & o);
protected: 
	/*! Maximum minor id 
	 */
	unsigned long long m_max_minor_id;
	/*! A database, which table is belongs to
	 */
	sad::db::Database * m_database;
	/*! Objects, determined by name
	 */
	sad::Hash<sad::String, sad::Vector<sad::db::Object*> > m_object_by_name;
	/*! A hash, storing objects by minor id
	 */
	sad::Hash<unsigned long long, sad::db::Object*> m_objects_by_minorid;
	/*! A hash, storing objects by major id
	 */
	sad::Hash<unsigned long long, sad::db::Object*> m_objects_by_majorid;
};

}

}


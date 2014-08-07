/*! \file db/database.h
	\author HiddenSeeker
	
	Contains definition of class Database.

 */
#pragma once
#include "../sadstring.h"
#include "dbobjectfactory.h"
#include "dberror.h"
#include "dbtable.h"

namespace sad
{

namespace db
{

/*! \class Database

 */
class Database  
{	
public:	
	/*! This class can be inherited 
	 */
	virtual ~Database();
	/*! 
		\return
	 */
	sad::String save();
	/*! 
		\param[in] s
	 */
	void saveToFile(const sad::String& s);
	/*! 
		\param[in] name
	 */
	void loadFromFile(const sad::String& name);
	/*! 
		\param[in] text
	 */
	void load(const sad::String& text);
	/*! 
		\param[in] name
		\param[in] table
	 */
	void addTable(const sad::String& name, sad::db::Table* table);
	/*! 
		\param[in] name
	 */
	void removeTable(const sad::String& name);
	/*! 
		\return
	 */
	unsigned long long uniqueMajorId(sad::db::Table * t);
	/*! 
		\param[in] id
		\return
	 */
	bool save(unsigned long long id);
	/*! 
		\param[in] id
		\param[in] name
		\return
	 */
	bool save(unsigned long long id, const sad::String& name);
	/*! 
		\param[in] name
		\param[in] id
		\return
	 */
	sad::Vector<sad::db::Error*> load(const sad::String& name, unsigned long long& id);
	/*! 
		\param[in] name
		\return
	 */
	sad::db::Table* table(const sad::String& name);
	/*! 
		\return
	 */
	sad::Hash<sad::String, sad::db::Table*>::iterator begin();
	/*! 
		\return
	 */
	sad::Hash<sad::String, sad::db::Table*>::iterator end();
	/*! 
		\return
	 */
	sad::db::ObjectFactory* factory();
	/*! 
		\param[in] f
	 */
	void setFactory(sad::db::ObjectFactory* f);
protected: 
	/*! 
	 */
	unsigned long long m_max_major_id;
	/*! 
	 */
	sad::Hash<sad::String, sad::db::Table*> m_tables;
	/*! 
	 */
	sad::db::ObjectFactory* m_factory;

};

}

}


/*! \file db/database.h
	\author HiddenSeeker
	
	Contains definition of class Database.

 */
#pragma once


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
	void saveToFile(sad::String s);
	/*! 
		\param[in] name
	 */
	void loadFromFile(sad::String name);
	/*! 
		\param[in] text
	 */
	void load(sad::String text);
	/*! 
		\param[in] name
		\param[in] table
	 */
	void addTable(sad::String name, db::Table* table);
	/*! 
		\param[in] name
	 */
	void removeTable(sad::String name);
	/*! 
		\return
	 */
	unsigned long long addMajor();
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
	bool save(unsigned long long id, sad::String name);
	/*! 
		\param[in] name
		\param[in] id
		\return
	 */
	sad::Vector<db::Error*> load(sad::String name, unsigned long long& id);
	/*! 
		\param[in] name
		\return
	 */
	db::Table* table(sad::String name);
	/*! 
		\return
	 */
	sad::Hash<sad::String, db::Table*>::iterator begin();
	/*! 
		\return
	 */
	sad::Hash<sad::String, db::Table*>::iterator end();
	/*! 
		\return
	 */
	db::ObjectFactory* factory();
	/*! 
		\param[in] f
	 */
	void setFactory(db::ObjectFactory* f);
protected: 
	/*! 
	 */
	unsigned long long m_max_major_id;
	/*! 
	 */
	sad::Hash<sad::String, db::Table*> m_tables;
	/*! 
	 */
	db::ObjectFactory* m_factory;

}

}

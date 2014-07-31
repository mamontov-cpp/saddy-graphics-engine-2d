/*! \file db/table.h
	\author HiddenSeeker
	
	Contains definition of class Table.

 */
#pragma once
#include "dbobject.h"

namespace sad
{

namespace db
{

/*! \class Table

 */
class Table  
{	
public:	
	/*! This class can be inherited 
	 */
	virtual ~Table();
	/*! 
		\param[in] a
	 */
	void add(sad::db::Object* a);
	/*! 
		\param[in] a
	 */
	void remove(sad::db::Object* a);
	/*! 
		\param[in] major_id
		\param[in] minor_Id
		\return
	 */
	sad::db::Object* queryByID(int major_id, int minor_Id);
	/*! 
		\param[in] name
		\return
	 */
	sad::Vector<sad::db::Object*> queryByName(const sad::String& name);
	/*! 
		\param[in] major_id
		\return
	 */
	sad::Vector<sad::db::Object*> queryByMajorId(int major_id);
protected: 
	/*! 
	 */
	sad::Vector<sad::db::Object*> m_objects;

};

}

}


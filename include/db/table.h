/*! \file db/table.h
	\author HiddenSeeker
	
	Contains definition of class Table.

 */
#pragma once


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
	void add(db::Object* a);
	/*! 
		\param[in] a
	 */
	void remove(db::Object* a);
	/*! 
		\param[in] major_id
		\param[in] minor_Id
		\return
	 */
	db::Object* queryByID(int major_id, int minor_Id);
	/*! 
		\param[in] name
		\return
	 */
	sad::Vector<db::Object*> queryByName(sad::String name);
	/*! 
		\param[in] major_id
		\return
	 */
	sad::Vector<db::Object*> queryByMajorId(int major_id);
protected: 
	/*! 
	 */
	sad::Vector<db::Object*> m_objects;

}

}

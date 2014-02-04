/*! \file db/object.h
	\author HiddenSeeker
	
	Contains definition of class Object.

 */
#pragma once


namespace db
{

/*! \class Object

 */
class Object  
{	
public:	
	/*! This class can be inherited 
	 */
	virtual ~Object();
	/*! 
		\return
	 */
	picojson::value save();
	/*! 
		\param[in] v
	 */
	void load(picojson::value v);
	/*! 
		\return
	 */
	db::Table* table();
	/*! 
		\param[in] t
	 */
	void setTable(db::Table* t);
	/*! 
	 */
	int MajorId;
	/*! 
	 */
	int MinorId;
	/*! 
	 */
	bool Active;
	/*! 
	 */
	int Sort;
	/*! 
	 */
	db::Table* Table;


}

}

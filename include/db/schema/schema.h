/*! \file db/schema/schema.h
	\author HiddenSeeker
	
	Contains definition of class Schema.

 */
#pragma once


namespace schema
{

/*! \class Schema

 */
class Schema: public db::Object  
{	
public:	
	/*! This class can be inherited 
	 */
	virtual ~Schema();
	/*! 
		\param[in] v
		\return
	 */
	bool check(picojson::value v);
	/*! 
		\param[in] v
	 */
	void load(picojson::value v);
	/*! 
		\return
	 */
	picojson::value save();
protected: 
	/*! 
	 */
	db::schema::PropertyType m_properties;

}

}

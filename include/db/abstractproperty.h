/*! \file db/abstractproperty.h
	\author HiddenSeeker
	
	Contains definition of class AbstractProperty.

 */
#pragma once


namespace db
{

/*! \class AbstractProperty

 */
class AbstractProperty  
{	
public:	
	/*! This class can be inherited 
	 */
	virtual ~AbstractProperty();
	/*! 
		\param[in] v
	 */
	void set(db::Variant v);
	/*! 
		\return
	 */
	db::Variant get();
	/*! 
		\return
	 */
	picojson::value save();
	/*! 
		\param[in] a
	 */
	void load(picojson::value a);
	/*! 
		\return
	 */
	bool saveable();
	/*! 
	 */
	void call();


}

}

/*! \file db/schema/propertytype.h
	\author HiddenSeeker
	
	Contains definition of class PropertyType.

 */
#pragma once


namespace schema
{

/*! \class PropertyType

 */
class PropertyType  
{	
public:	
	/*! This class can be inherited 
	 */
	virtual ~PropertyType();
	/*! 
		\param[in] v
		\return
	 */
	virtual bool check(picojson::value v) = 0;;


}

}

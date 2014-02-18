/*! \file db/objectfactory.h
	\author HiddenSeeker
	
	Contains definition of class ObjectFactory.

 */
#pragma once


namespace db
{

/*! \class ObjectFactory

 */
class ObjectFactory  
{	
public:	
	/*! This class can be inherited 
	 */
	virtual ~ObjectFactory();
	/*! 
		\return
	 */
	db::Object* create();


}

}

/*! \file resource/resourcefactory.h
	\author HiddenSeeker
	
	Contains definition of class ResourceFactory.

 */
#pragma once


namespace resource
{

/*! \class ResourceFactory

 */
class ResourceFactory  
{	
public:	
	/*! This class can be inherited 
	 */
	virtual ~ResourceFactory();
	/*! 
		\param[in] sad
		\return
	 */
	resource::Resource* create(sad::String sad);


}

}

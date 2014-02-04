/*! \file resource/link.h
	\author HiddenSeeker
	
	Contains definition of class Link.

 */
#pragma once


namespace resource
{

/*! \class Link

 */
class Link<
	typename _Resource
>: public resource::AbstractLink  
{	
public:	
	/*! 
	 */
	Link()
	{
	
	}
	
	/*! This class can be inherited 
	 */
	virtual ~Link()
	{
	
	}
	/*! 
		\return
	 */
	_Resource* resource()
	{
		return _Resource*();	
	}


}

}

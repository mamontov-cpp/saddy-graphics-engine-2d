/*! \file resource/link.h
	\author HiddenSeeker
	
	Contains definition of class Link.

 */
#pragma once
#include "abstractlink.h"

namespace resource
{

/*! \class Link

 */
template<
	typename _Resource
>
class Link: public resource::AbstractLink  
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

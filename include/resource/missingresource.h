/*! \file resource/missingresource.h
	\author HiddenSeeker
	
	Contains definition of class MissingResource.

 */
#pragma once


namespace resource
{

/*! \class MissingResource

 */
class MissingResource: public resource::Error  
{	
public:	
	/*! This class can be inherited 
	 */
	virtual ~MissingResource();


}

}

/*! \file resource/link.h
	\author HiddenSeeker
	
	Contains definition of class Link.

	Defines a link to specific type to resource, providing simple access to it.
 */
#pragma once
#include "abstractlink.h"
#include "resource.h"

namespace sad
{

namespace resource
{

/*! \class Link

	Defines a link to specific type to resource, providing simple access to it.
 */
template<
	typename _Resource
>
class Link: public sad::resource::AbstractLink  
{	
public:	
	/*! Creates new link to specified resource
	 */
	Link() : sad::resource::AbstractLink(_Resource::globalMetaData()->name())
	{
	
	}
	/*! Returns resource of specific type. No run-time checks are performed due
		possible errors
		\return	resource of specific type
	 */
	inline _Resource* get() const
	{
		return static_cast<_Resource*>(this->resource());	
	}
};

}

}

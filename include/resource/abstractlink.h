/*! \file resource/abstractlink.h
	\author HiddenSeeker
	
	Contains definition of class AbstractLink.

 */
#pragma once


namespace resource
{

/*! \class AbstractLink

 */
class AbstractLink  
{	
public:	
	/*! 
		\param[in] resourceType
	 */
	AbstractLink(sad::String resourceType);
	/*! This class can be inherited 
	 */
	virtual ~AbstractLink();
	/*! 
		\param[in] r
	 */
	void attach(resource::Resource* r);
	/*! 
	 */
	void detach();
	/*! 
		\param[in] tree
		\return
	 */
	bool valid(resource::Tree* tree);
	/*! 
		\return
	 */
	resource::Resource* resource();
	/*! 
		\param[in] path
	 */
	void setPath(sad::String path);
	/*! 
		\return
	 */
	sad::String path();
protected: 
	/*! 
	 */
	sad::String m_path;
	/*! 
	 */
	resource::Resource* m_resource;

}

}

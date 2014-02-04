/*! \file resource/resource.h
	\author HiddenSeeker
	
	Contains definition of class Resource.

 */
#pragma once


namespace resource
{

/*! \class Resource

 */
class Resource: public sad::Object  
{	
public:	
	/*! This class can be inherited 
	 */
	virtual ~Resource();
	/*! 
		\return
	 */
	sad::String resourceType();
	/*! 
		\param[in] file
	 */
	void load(resource::PhysicalFile file);
	/*! 
		\param[in] a
	 */
	void replaceWith(resource::Resource* a);
	/*! 
		\return
	 */
	sad::Vector<resource::Error*> reload();
	/*! 
		\param[in] folder
	 */
	void setParentFolder(resource::Folder* folder);
	/*! 
		\return
	 */
	resource::Folder* parentFolder();
	/*! 
		\return
	 */
	sad::String name();
protected: 
	/*! 
	 */
	resource::Folder m_folder;
private: 
	/*! 
	 */
	sad::Vector<resource::AbstractLink*> m_links;
}

}

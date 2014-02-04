/*! \file resource/folder.h
	\author HiddenSeeker
	
	Contains definition of class Folder.

 */
#pragma once
#include <../sadstring.h>
#include <../sadhash.h>

namespace resource
{
class Resource;

/*! \class Folder

 */
class Folder  
{	
public:	
	/*! 
	 */
	Folder();
	/*! This class can be inherited 
	 */
	virtual ~Folder();
	/*! 
		\param[in] name
		\param[in] folder
		\return
	 */
	bool addFolder(sad::String name, resource::Folder* folder);
	/*! 
		\param[in] name
		\param[in] folder
		\return
	 */
	bool addResource(sad::String name, resource::Folder* folder);
	/*! 
		\param[in] name
	 */
	void removeFolder(sad::String name);
	/*! 
		\param[in] name
	 */
	void removeResource(sad::String name);
	/*! 
		\param[in] name
		\return
	 */
	resource::Folder* folder(sad::String name);
	/*! 
		\param[in] name
		\return
	 */
	resource::Resource* resource(sad::String name);
	/*! 
		\param[in] name
	 */
	void setName(sad::String name);
	/*! 
		\return
	 */
	sad::String name();
	/*! 
		\param[in] name
		\param[in] r
	 */
	void replaceResource(sad::String name, resource::Resource* r);
	/*! 
		\return
	 */
	sad::Hash<sad::String, resource::Folder*>::iterator foldersBegin();
	/*! 
		\return
	 */
	sad::Hash<sad::String, resource::Folder*>::iterator foldersEnd();
	/*! 
		\return
	 */
	sad::Hash<sad::String, resource::Resource*>::iterator resourceBegin();
	/*! 
		\return
	 */
	sad::Hash<sad::String, resource::Resource*>::iterator resourceEnd();
protected: 
	/*! 
	 */
	sad::Hash<sad::String, resource::Folder*> m_subfolders;
	/*! 
	 */
	sad::Hash<sad::String, resource::Resource*> m_resources;
	/*! 
	 */
	sad::String m_name;

};

}

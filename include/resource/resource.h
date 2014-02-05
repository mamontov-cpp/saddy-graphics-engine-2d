/*! \file resource/resource.h
	\author HiddenSeeker
	
	Contains definition of class Resource.

	A resource is a basic object, which can be loaded from file and stored in resource databse
	and have dependency on it.
 */
#pragma once
#include "../object.h"
#include "../sadstring.h"
#include "../3rdparty/picojson/picojson.h"

namespace sad
{
class Renderer;

namespace resource
{
class PhysicalFile;
class Error;
class Folder;
class AbstractLink;

/*! \class Resource

	A resource is a basic object, which can be loaded from file and stored in resource databse
	and have dependency on it.
 */
class Resource: public sad::Object  
{	
SAD_OBJECT
public:
	/*! Inits new resource, which does not belong to any folder
	 */
	Resource();
	/*! This class can be inherited 
	 */
	virtual ~Resource();
	/*! Loads a resource from specified file, using specified renderer for resolving some 
		properties.
		\param[in] file a file, via which a resource should be loaded
		\param[in] r  a renderer, which resource should be linked to (NULL if global renderer)
		\param[in] v  an options for loading a resource
		\param[in] store_links hints, whether we should store a links
	 */
	virtual bool load(
		const sad::resource::PhysicalFile & file,
		sad::Renderer * r = NULL,
		const picojson::value& options = picojson::value(picojson::object_type, false),
		bool store_links = true
	) = 0;
	/*! Returns true, if resource was referenced
		\return true if resource is referenced
	 */
	bool referenced() const;
	/*! Copies all links from current instance to other resource
		\param[in] a new resource
	 */
	void replaceWith(sad::resource::Resource* a);
	/*! Sets parent folder for a resource
		\param[in] folder a parent folder for resource
	 */
	void setParentFolder(sad::resource::Folder* folder);
	/*! Returns parent folder for a resource
		\return a parent folder for a resource
	 */
	sad::resource::Folder* parentFolder();
	/*! Returns name for a resource. Names must be unique in this folder
		\return name for a resource
	 */
	const sad::String & name();
	/*! Sets name for resource
		\param[in] name a name to be set
	 */
	void setName(const sad::String & name);
	/*! Adds link to resource. Note, that link is not notified about it.
		\param[in] link a link to resource
	 */
	void addLink(sad::resource::AbstractLink* link);
	/*! Removes link from resource. Note, that link is not notified about it.
		\param[in] link a link to resource
	 */
	void removeLink(sad::resource::AbstractLink* link);
	/*! Enables storing links to resource in resource class
	 */
	void enableStoringLinks();
	/*! Disables storing links to resource in resource class.
		Also clears the pointers to links, stored in resource.
	 */
	void disableStoringLinks();
	/*! Sets physical file, where resource is stored
		\param[in] file a file data
	 */
	void setPhysicalFile(sad::resource::PhysicalFile * file);
	/*! Returns a physical fle, where resource supposedly stored
		\return file
	 */
	sad::resource::PhysicalFile * file() const;
protected: 
	/*! A parent folder of resoruce
	 */
	sad::resource::Folder* m_folder;
	/*! A physical file, where resouces are stored
	 */
	sad::resource::PhysicalFile * m_file;
	/*! Whether we should store links to resources
	 */
	bool m_store_links;
	/*! Returns name for resource
	 */
	sad::String m_name;
	/*! A links to a resource
	 */
	sad::Vector<sad::resource::AbstractLink*> m_links;
};

}

}


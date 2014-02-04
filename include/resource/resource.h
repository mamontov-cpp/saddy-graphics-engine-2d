/*! \file resource/resource.h
	\author HiddenSeeker
	
	Contains definition of class Resource.

	A resource is a basic object, which can be loaded from file and stored in resource databse
	and have dependency on it.
 */
#pragma once
#include "../object.h"
#include "../sadstring.h"

namespace sad
{
class Renderer;
}

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
	/*! A resources in factory should be created via resourceType() hint. So every resource type
		should implement it
		\return name for resource type. Must be unique.
	 */
	virtual sad::String resourceType() = 0;
	/*! Loads a resource from specified file, using specified renderer for resolving some 
		properties.
		\param[in] file a file, via which a resource should be loaded
		\param[in] r  a renderer, which resource should be linked to (NULL if global renderer)
		\param[in] store_links hints, whether we should store a links
	 */
	virtual bool load(
		const resource::PhysicalFile & file,
		sad::Renderer * r = NULL,
		bool store_links = true
	) = 0;
	/*! Returns true, if resource was referenced
		\return true if resource is referenced
	 */
	bool referenced() const;
	/*! Copies all links from current instance to other resource
		\param[in] a new resource
	 */
	void replaceWith(resource::Resource* a);
	/*! Sets parent folder for a resource
		\param[in] folder a parent folder for resource
	 */
	void setParentFolder(resource::Folder* folder);
	/*! Returns parent folder for a resource
		\return a parent folder for a resource
	 */
	resource::Folder* parentFolder();
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
	void addLink(resource::AbstractLink* link);
	/*! Removes link from resource. Note, that link is not notified about it.
		\param[in] link a link to resource
	 */
	void removeLink(resource::AbstractLink* link);
protected: 
	/*! A parent folder of resoruce
	 */
	resource::Folder* m_folder;
	/*! Whether we should store links to resources
	 */
	bool m_store_links;
	/*! Returns name for resource
	 */
	sad::String m_name;
	/*! A links to a resource
	 */
	sad::Vector<resource::AbstractLink*> m_links;
};

}

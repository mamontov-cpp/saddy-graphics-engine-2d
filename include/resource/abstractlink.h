/*! \file resource/abstractlink.h
	\author HiddenSeeker
	
	Contains definition of class AbstractLink.

	An abstract link links resources from resource database to an objects, also
	handling reloading of objects.
 */
#pragma once
#include "../sadstring.h"

namespace resource
{
class Resource;
class Tree;

/*! \class AbstractLink

	An abstract link links resources from resource database to an objects, also
	handling reloading of objects.
 */
class AbstractLink  
{	
public:	
	/*! Constructs new link by resource type
		\param[in] resource_type a basic resource type
	 */
	AbstractLink(const sad::String & resource_type);
	/*! This class can be inherited 
	 */
	virtual ~AbstractLink();
	/*! Attach a link to specified resource
		\param[in] r a new resource, which links should be attached to
	 */
	void attach(resource::Resource* r);
	/*! Sets resource link to object to NULL
	 */
	void detach();
	/*! Lazily fatches a resource
		\return fetched resource
	 */
	resource::Resource* resource() const;
	/*! Sets path to an abstract link
		\param[in] path path to a linked resource
	 */
	void setPath(sad::String path);
	/*! Returns path to a linked resource
		\return path to a linked resource
	 */
	const sad::String & path() const;
	/*! Sets changed flag to false, notifying that we finished changing
	 */
	void finishedChanging();
	/*! Returns flag, whether we are changed a link
		\return whether link is changed
	 */
	bool changed() const;
	/*! Sets a tree for a link
		\param[in] tree a tree for link
	 */
	void setTree(resource::Tree * tree);
	/*! Returns tree, which link references to
		\return tree for a link
	 */
	resource::Tree* tree() const;
protected:
	/*! Whether user handled change
	 */
	bool m_changed;
	/*! 
	 */
	sad::String m_path;
	/*! 
	 */
	resource::Resource* m_resource;
	/*! A tree, where resource is stored
	 */
	resource::Tree *   m_tree;
};

}

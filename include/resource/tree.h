/*! \file resource/tree.h
	\author HiddenSeeker
	
	Contains definition of class Tree.

	A tree stores all resources of game. The game can have several separate trees, but it must 
	have at least one.
 */
#pragma once
#include "../sadptrvector.h"
#include "resource.h"
#include "resourcefactory.h"
#include "folder.h"
#include "error.h"

namespace sad
{
class Renderer;

namespace resource
{

/*! \class Tree

	A tree stores all resources of game. The game can have several separate trees, but it must 
	have at least one.
 */
class Tree  
{	
public:	
	/*! Constructs a new empty tree
		\param[in] r used renderer (NULL for global)
	 */
	Tree(sad::Renderer * r = NULL);
	/*! This class can be inherited 
	 */
	virtual ~Tree();
	/*! Loads a tree from a string, adding to existing data new stored resources if new errors
		occured.
		\param[in] string
		\return list of errors
	 */
	virtual sad::Vector<sad::resource::Error*> loadFromString(const sad::String & string);
	/*! Loads a tree from a file, adding to existing data new stored resources if new errors
		occured.
		\param[in] string
		\return list of errors
	 */
	sad::Vector<sad::resource::Error*> loadFromFile(const sad::String& string);
	/*! Loads new file. If no errors found, all resources will be stored in
		node.

		\param[in] typehint a hint for type of file to be loaded
		\param[in] filename a name of file to be loaded
		\param[in] resourcename a name of resource, if any should be loaded
	*/
	sad::Vector<sad::resource::Error*> load(
		const sad::String& typehint, 
		const sad::String& filename, 
		const sad::Maybe<sad::String>& resourcename
	);
	/*! Loads new file. If no errors found, all resources will be stored in
		node.

		\param[in] typehint a hint for type of file to be loaded
		\param[in] filename a name of file to be loaded
		\param[in] resourcename a name of resource, if any should be loaded
		\param[in] store where new resources, should be stored (NULL for current root)
		\param[in] v  a parsed JSON value
		\param[out] files list of files, where new resources should be stored
		\return list of errors
	 */
	virtual sad::Vector<sad::resource::Error*> load(
		const sad::String& typehint, 
		const sad::String& filename, 
		const sad::Maybe<sad::String>& resourcename,
		sad::resource::Folder * store,
		const picojson::value & v,
		sad::Vector<sad::resource::PhysicalFile *> & files		
	);
	/*! Unloads file, removing all resources from it and freeing it's memory. 
		Can fail if some of resources are referenced
		\param[in] file a file to be unloaded
		\return whether it was successfull
	 */
	bool unload(const sad::String& file);
	/*! Unloads file, removing all resources from it and freeing it's memory. 
		Can fail if some of resources are referenced
		\param[in] file a file to be unloaded
		\return whether it was successfull
	 */
	bool unload(sad::resource::PhysicalFile * file);
	/*! A root folder of tree
		\return a root folder of tree
	 */
	sad::resource::Folder* root() const;
	/*! Returns registered file by a name
		\param[in] name a file name
		\return file or NULL if not found
	 */
	sad::resource::PhysicalFile* file(const sad::String& name);
	/*! Returns a vector of files
		\return vector of files
	 */
	const sad::Vector<sad::resource::PhysicalFile*>& files() const; 
	/*! Returns a factory for a tree
		\return a factory
	 */
	sad::resource::Factory* factory();
	/*! Sets new factory for a tree
		\param[in] factory a factory to be stored
	 */
	void setFactory(sad::resource::Factory* factory);
	/*! Sets renderer 
		\param[in] renderer used renderer
	 */
	void setRenderer(sad::Renderer * renderer);
	/*! Returns used renderer
		\return a used renderer
	 */ 
	sad::Renderer * renderer() const;
	/*! Whether resource should store links
		\return whether we should store links
	 */
	bool shouldStoreLinks() const;
	/*! Determines whether new resources should store links
		\param[in] store a whether links to resources should be stored.
	 */
	void setStoreLinks(bool store);
	/*! Converts duplicates to errors, that resource already exists
		\param[in] l a resource entry list
		\return error list
	 */
	sad::Vector<sad::resource::Error *> duplicatesToErrors(
		const sad::Vector<sad::String> & l
	);
protected: 
	/*! A loaded files to be stored
	 */
	sad::PtrVector<sad::resource::PhysicalFile> m_files;
	/*! A root folder
	 */
	sad::resource::Folder* m_root;
	/*! A factory to be used
	 */
	sad::resource::Factory* m_factory;
	/*! Used renderer data
	 */
	sad::Renderer * m_renderer;
	/*! Whether we should store links
	 */
	bool m_storelinks;
private:
	/*! Disabled, tree is uncopyable
		\param[in] o other tree
	 */
	Tree(const Tree & o);
	/*! Disabled, tree is uncopyable
		\param[in] o other tree
		\return self-reference
	 */
	Tree & operator=(const Tree & o);
};

}

}

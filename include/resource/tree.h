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
	/*! Inits a tree from a string, loading resources, stored in JSON in string
		\param[in] string
		\return whether it was successfull
	 */
	virtual bool initFromString(const sad::String & string);
	/*! Inits a tree, loading a resources from a file, where data stored here is successfull
		\param[in] string
		\return whether it was successfull
	 */
	bool initFromFile(const sad::String& string);
	/*! Loads new file, trying to load it
		\param[in] typehint a hint for type of file to be loaded
		\param[in] file a name of file to be loaded
		\param[in] name a name of resource
		\return whether it was successfull
	 */
	virtual bool load(const sad::String& typehint, const sad::String& file, const sad::String& name);
	/*! Unloads file, removing all resources from it and freeing it's memory. 
		Can fail if some of resources are referenced
		\param[in] file a file to be unloaded
		\return whether it was successfull
	 */
	bool unload(const sad::String& file);
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
		\param[in] a whether links should be stored.
	 */
	void setStoreLinks(bool store);
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

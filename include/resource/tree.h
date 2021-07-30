/*! \file resource/tree.h
    
    
    Contains definition of class Tree.

    A tree stores all resources of game. The game can have several separate trees, but it must 
    have at least one.
 */
#pragma once
#include "../sadptrvector.h"
#include "../sadhash.h"
#include "../refcountable.h"
#include "resource.h"
#include "resourcefactory.h"
#include "folder.h"
#include "error.h"


namespace tar7z
{

class Archive;
class Entry;

}

namespace sad
{
class Renderer;

namespace resource
{

/*! \class Tree

    A tree stores all resources of game. The game can have several separate trees, but it must 
    have at least one.
 */
class Tree: public sad::RefCountable  
{	
public:	
    /*! Constructs a new empty tree
        \param[in] r used renderer (nullptr for global)
     */
    Tree(sad::Renderer * r = nullptr);
    /*! This class can be inherited 
     */
    virtual ~Tree() override;
    /*! Loads a tree from a string, adding to existing data new stored resources if new errors
        occurred.
        \param[in] string a string with content
        \return list of errors
     */
    virtual sad::Vector<sad::resource::Error*> loadFromString(const sad::String & string);
    /*! Loads a tree from a string, adding to existing data new stored resources if new errors
        occurred.
        \param[in] string a string with content
        \return maybe error message
     */
    sad::Maybe<sad::String> tryLoadFromString(const sad::String & string);
    /*! Loads a tree from a file, adding to existing data new stored resources if new errors
        occurred.
        \param[in] string a file name
        \return list of errors
     */
    sad::Vector<sad::resource::Error*> loadFromFile(const sad::String& string);
    /*! Loads a tree from a string, adding to existing data new stored resources if new errors
        occurred.
        \param[in] string a file name
        \return maybe error message
     */
    sad::Maybe<sad::String> tryLoadFromFile(const sad::String & string);
    /*! Loads new file. If no errors found, all resources will be stored in
        node.

        \param[in] type_hint a hint for type of file to be loaded
        \param[in] filename a name of file to be loaded
        \param[in] resource_name a name of resource, if any should be loaded
    */
    sad::Vector<sad::resource::Error*> load(
        const sad::String& type_hint, 
        const sad::String& filename, 
        const sad::Maybe<sad::String>& resource_name
    );
    /*! Loads new file. If no errors found, all resources will be stored in
        node.

        \param[in] type_hint a hint for type of file to be loaded
        \param[in] filename a name of file to be loaded
        \param[in] resource_name a name of resource, if any should be loaded
        \param[in] store where new resources, should be stored (nullptr for current root)
        \param[in] v  a parsed JSON value
        \param[out] files list of files, where new resources should be stored
        \return list of errors
     */
    virtual sad::Vector<sad::resource::Error*> load(
        const sad::String& type_hint, 
        const sad::String& filename, 
        const sad::Maybe<sad::String>& resource_name,
        sad::resource::Folder * store,
        const picojson::value & v,
        sad::Vector<sad::resource::ResourceFile *> & files		
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
    bool unload(sad::resource::ResourceFile * file);
    /*! A root folder of tree
        \return a root folder of tree
     */
    sad::resource::Folder* root() const;
    /*! Returns registered file by a name
        \param[in] name a file name
        \return file or nullptr if not found
     */
    sad::resource::ResourceFile* file(const sad::String& name);
    /*! Returns a vector of files
        \return vector of files
     */
    const sad::Vector<sad::resource::ResourceFile*>& files() const; 
    /*! Returns a factory for a tree
        \return a factory
     */
    sad::resource::Factory* factory() const;
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
    /*! Returns resource, casted to type
        \param[in] name a name of resource
        \return result
     */
    template<
        typename _ResourceType
    >
    _ResourceType* get(const sad::String & name)
    {
        _ResourceType* result = nullptr;
        sad::resource::Resource* r = root()->resource(name);
        if (r)
        {
            result = r->as<_ResourceType>();
        }
        return result;
    }

    /*! Returns a temporary root folder, when loading all files
        \return temporary root folder
     */
    sad::resource::Folder * temporaryRoot() const;
    /*! Forces all resources to unload self from GPU
     */
    void unloadResourcesFromGPU();
    /*! Returns archive entry by name. Tries to load archive if need to
        \param[in] archive an archive name
        \param[in] filename a file name
        \param[in] load_new force loading new archive
        \return entry if entry exists
     */
    tar7z::Entry* archiveEntry(const sad::String& archive, const sad::String& filename, bool load_new = false);
protected:
    /*! An archive list
     */
    sad::Vector<tar7z::Archive*> m_archive_list;
    /*! An archives for a tree
     */
    sad::Hash<sad::String, unsigned int> m_archives;
    /*! A loaded files to be stored
     */
    sad::PtrVector<sad::resource::ResourceFile> m_files;
    /*! A root folder
     */
    sad::resource::Folder* m_root;
    /*! A temporary root when loading
     */
    sad::resource::Folder * m_temporary_root_folder;
    /*! A factory to be used
     */
    sad::resource::Factory* m_factory;
    /*! Used renderer data
     */
    sad::Renderer * m_renderer;
    /*! Whether we should store links
     */
    bool m_store_links;
private:
    /*! A current root for loading data
     */
    sad::String m_current_root;
    /*! A temporary root for loading data
     */
    sad::String m_temporary_root;
    /*! Disabled, tree is un-copyable
        \param[in] o other tree
     */
    Tree(const Tree & o);
    /*! Disabled, tree is un-copyable
        \param[in] o other tree
        \return self-reference
     */
    Tree & operator=(const Tree & o);
};

}

}

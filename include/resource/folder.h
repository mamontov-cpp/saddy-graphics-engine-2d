/*! \file resource/folder.h
    
    
    Contains definition of class Folder.

    A folder may contain resources and subfolders, behaving like leaf of tree
    of resources storing all data. Note, that adding new resource to non-existing
    subfolders causes creating new subfolders.
 */
#pragma once
#include "../maybe.h"
#include "../sadpair.h"
#include "../sadstring.h"
#include "../sadptrhash.h"

namespace sad
{

namespace resource
{

class Folder;
class Resource;
/*! An iterator for iterating on resources of folder
 */
typedef sad::Hash<sad::String, sad::resource::Resource*>::iterator ResourceIterator;
/*! An iterator for iterating on subfolders of folder
 */
typedef sad::Hash<sad::String, sad::resource::Folder*>::iterator FolderIterator;
/*! An entry of resource, stored in folder
 */
typedef sad::Pair<sad::String, sad::resource::Resource*> ResourceEntry;
/*! A list of resource entry, stored in folder
 */
typedef sad::Vector<sad::resource::ResourceEntry> ResourceEntryList;
/*! Frees a resource list
    \param[in] list a list of resources
 */
void free(const sad::resource::ResourceEntryList & list);
/*! \class Folder

    A folder may contain resources and subfolders, behaving like leaf of tree
    of resources storing all data. Note, that adding new resource to non-existing
    subfolders causes creating new subfolders.
 */
class Folder  
{	
public:	
    /*! Creates new empty folder.
     */
    Folder();
    /*! This class can be inherited 
     */
    virtual ~Folder();
    /*! Returns, whether folder has subfolders
        \return whether folder has subfolders
     */
    bool hasFolders() const;
    /*! Returns, whether folder has resources
        \return whether folder has resources
     */
    bool hasResources() const;
    /*! Adds new subfolder with specified name to current folder. Note that old folder will
        be replaced if it exists, destroying old linked resources.
        \param[in] path	a path to subfolder, including  it's name, using "/" as separator.
        \param[in] folder a subfolder to be added
        \return whether adding was successfull 
                (false if name is empty, or  count of separators is bigger than 1024).
     */
    bool addFolder(const sad::String&  path, sad::resource::Folder* folder);
    /*! Copies a resource list to a folder
        \param[in] list a list
        \param[in] ref whether we should reference this resource
        \return whether all resources are added successfully
     */
    bool addResources(const sad::resource::ResourceEntryList & list, bool ref = true);
    /*! Replaces a resource list
        \param[in] list a list
     */
    void replaceResources(const sad::resource::ResourceEntryList & list);
    /*! Removes a resource list, freeing or not a data
        \param[in] list a list
        \param[in] free whether we should free a resource
     */
    void removeResources(const sad::resource::ResourceEntryList & list, bool free);
    /*! Returns list of resource names, which exists in both in folder and list
        \param[in] list a list of resources
        \return list of items
     */
    sad::Vector<sad::String> duplicatesBetween(const sad::resource::ResourceEntryList & list);
    /*! Adds new sub-resource with specified name to current folder
        \param[in] path a path to resource, including  it's name, using "/" as separator 
        \param[in] res a resource to be added
        \param[in] ref whether we should reference this resource
        \return whether adding was successfull 
                (false if name is empty, or  count of separators is bigger than 1024).

     */
    bool addResource(const sad::String& path, sad::resource::Resource* res, bool ref = true);
    /*! Removes a subfolder with specified path, freeing it's memory from path
        \param[in] path a path to folder, including it's name, using "/" as separator
        \param[in] free whether we should free memory from folder, erasing some data
     */
    void removeFolder(const sad::String& path, bool free = true);
    /*! Removes a resource with specified path, freeing it's memory from path
        \param[in] path a path to resource
        \param[in] free whether we should free memory from folder, erasing some data
     */
    void removeResource(const sad::String& path, bool free = true);
    /*! Returns a sub-folder  by specified path, or null if not found
        \param[in] name a path to folder, including it's name, using "/" as separator
        \return a folder
     */
    sad::resource::Folder* folder(const sad::String& name);
    /*! Returns a sub-folder  by specified path, or null if not found
        \param[in] name a path to resource, including it's name, using "/" as separator
        \return a folder
     */
    sad::resource::Resource* resource(const sad::String& name);
    /*! Replaces a resource, defined by path with another resource, removing old resource 
        (if exists).
        \param[in] path a name of resource, using "/" as separator
        \param[in] r a resource
     */
    void replaceResource(const sad::String& path, sad::resource::Resource* r);
    /*! Returns begin iterator for folders
        \return iterator for folders
     */
    sad::resource::FolderIterator folderListBegin();
    /*! Returns end iterator for folders
        \return iterator for folders
     */
    sad::resource::FolderIterator folderListEnd();
    /*! Returns begin iterator for resources
        \return begin iterator for resources
     */
    sad::resource::ResourceIterator resourceListBegin();
    /*! Returns end iterator for resources
        \return end iterator for resources
     */
    sad::resource::ResourceIterator resourceListEnd();
    /*! Returns count of subfolders in folder
        \return count of subfolders in folder
     */
    unsigned int foldersCount() const;
    /*! Returns count of resources in folder
        \return count of resources in folder
     */
    unsigned int resourceCount() const;
    /*! A parent folder for current folder nullptr for top-most
        \param[in] folder parent folder
     */
    void setParent(sad::resource::Folder * folder);
    /*! Returns parent folder
        \return parent folder
     */
    sad::resource::Folder * parent() const;
    /*! Finds resource path by resource name
        \param[in] r resource to be found
        \return a path to resource if found
     */
    sad::Maybe<sad::String> find(sad::resource::Resource * r);
    /*! Recursively copies entities stored in folder in list, clearing data in 
        folders recursively
        \return resource list
     */
    ResourceEntryList copyAndClear();
    /*! Forces all resources to unload self from GPU
     */
    void unloadResourcesFromGPU();
protected: 
    /*! Navigates to parent folder of resource of folder named full path
        \param[in] path a path to folder or resource
        \param[in] create   whether we can create subfolders
        \param[out] name extracted name of resource or folder
     */
    sad::resource::Folder * navigateParentFolder(
        const sad::String & path, 
        bool create,
        sad::String & name
    );
    /*! A subfolders, stored in folder
     */
    sad::PtrHash<sad::String, sad::resource::Folder> m_subfolders;
    /*! A resources, stored in folder
     */
    sad::PtrHash<sad::String, sad::resource::Resource> m_resources;
    /*! A parent folder for current folder (nullptr for top)
     */
    sad::resource::Folder * m_parent;
};

}

}

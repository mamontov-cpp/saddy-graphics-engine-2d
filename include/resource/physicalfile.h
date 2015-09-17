/*! \file resource/physicalfile.h
    
    
    Contains definition of class PhysicalFile.

    This is a physical file, where resources were stored before loading, and where they are belong.
    So you can try to reload it, replacing resources with new ones.
 */
#pragma once
#include "../sadstring.h"
#include "../sadvector.h"
#include "error.h"

namespace sad
{

namespace resource
{
class Tree;
class Resource;
class Folder;

/*! An entry of resource, stored in folder
 */
typedef sad::Pair<sad::String, sad::resource::Resource*> ResourceEntry;
/*! A list of resource entry, stored in folder
 */
typedef sad::Vector<sad::resource::ResourceEntry> ResourceEntryList;

/*! \class PhysicalFile

    This is a physical file, where resources were stored before loading, and where they are belong.
    So you can try to reload it, replacing resources with new ones.
 */
class PhysicalFile  
{	
public:	
    /*! Creates new flle with specified name. Supposedly it must be path to specified file.
        \param[in] name a filename (with or without path) to it
     */
    PhysicalFile(const sad::String& name = "");	
    /*! This class does not own any of resources, only tree frees a resources
     */
    virtual ~PhysicalFile();
    /*! Returns true if file is anonymous
        \return whether file is anonymous
     */
    bool isAnonymous() const;
    /*! Returns name of file (with or without path)
        \return name of file (with or without path)
     */
    const sad::String & name() const;
    /*! Sets name of file (with or without path)
        \param[in] name name of file (with or without path)
     */
    void setName(const sad::String& name);
    /*! Loads a physical file. By default only returns false, since resource
        should be loaded only.

        Default implementation only returns error. Implement your own type of files, if
        you need some non-standard resources, like spritesheets or something.

        \param[in] parent a parent folder, where all created resources should be stored
     */
    virtual sad::Vector<sad::resource::Error*> load(sad::resource::Folder * parent);
    /*! Reloads all resources from a file
        \return errors if any occured on resources
     */
    virtual sad::Vector<sad::resource::Error*> reload();
    /*! Adds resource to file
        \param[in] r a resource
     */
    void add(sad::resource::Resource * r);
    /*! Removes a resource from registered resources of file
        \param[in] r a resource
     */
    void remove(sad::resource::Resource * r);
    /*! Sets tree for file
        \param[in] tree a tree
     */
    void setTree(sad::resource::Tree * tree);
    /*! Returns a tree for a file
        \return a tree for file
     */
    sad::resource::Tree * tree() const;
    /*! Returns a resources
        \return a resource
     */
    const sad::Vector<sad::resource::Resource*> & resources() const;
    /*! Replaces links to resource from to resource to.
        \param[in] from first resource
        \param[in] to second resource
     */
    void replace(sad::resource::Resource * from, sad::resource::Resource * to);
protected: 
    /*! Tries to read a file to string
     */
    sad::Maybe<sad::String> tryReadToString();
    /*! Replaces resources of texture atlas file with list
        \param[in] resourcelist a list of resources
     */
    void replaceResources(
        const sad::resource::ResourceEntryList & resourcelist
    );
    /*! Fills resources with resources, stored in this file
        \param[out] resources a resource list
     */
    void createOldResourceList(
        sad::resource::ResourceEntryList & resources
    );
    /*! Computes differences between two resource lists
        \param[in] oldlist an old list of resources
        \param[in] newlist a  new list of resources
        \param[out] tobeadded a resources, that should be added to tree
        \param[out] tobereplaced a resources from new list, that should replace old list
        \param[out] toberemoved a resources from old list, that should be removed
     */
    void diffResourcesLists(
        const sad::resource::ResourceEntryList & oldlist,
        const sad::resource::ResourceEntryList & newlist,
        sad::resource::ResourceEntryList & tobeadded,
        sad::resource::ResourceEntryList & tobereplaced,
        sad::resource::ResourceEntryList & toberemoved
    );
    /*! Converts referenced options to be removed to CannotDeleteReferencedResource errors, 
        appending them to a vector
        \param[in] toberemoved a list of resources
        \param[in] errors an occured errors
     */
    void convertReferencedOptionsToBeRemovedToErrors(
        const sad::resource::ResourceEntryList & toberemoved,
        sad::Vector<sad::resource::Error *> & errors
    );
    /*! A file name (with or without path), where file is stored
     */
    sad::String m_name;
    /*! A tree, where all resources are stored
     */
    sad::resource::Tree* m_tree;
    /*!  A resources, linked to file
     */
    sad::Vector<sad::resource::Resource*> m_resources;
};

/*! Frees list of errors
    \param[in] vector a vector of errors 
 */
inline void free_vector(const  sad::Vector<sad::resource::Error*> & vector)
{
    for(size_t i = 0; i < vector.size();i++)
    {
        delete vector[i];
    }
}

}

}

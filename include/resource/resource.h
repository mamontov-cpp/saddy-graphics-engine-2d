/*! \file resource/resource.h
    
    
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
class ResourceFile;
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
        \param[in] r  a renderer, which resource should be linked to (nullptr if global renderer)
        \param[in] options  an options for loading a resource
        \param[in] store_links hints, whether we should store a links
        \return whether loading was successfull
     */
    virtual bool tryLoad(
        const sad::resource::ResourceFile & file,
        sad::Renderer * r = nullptr,
        const picojson::value& options = picojson::value(picojson::object_type, false),
        bool store_links = true
    );
    /*! Unloads all resources from GPU. By default does nothing
     */
    virtual void unloadFromGPU();
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
    virtual void setParentFolder(sad::resource::Folder* folder);
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
    /*! Checks, whether resource should store links
        \return whether resource should store links
     */
    bool shouldStoreLinks() const;
    /*! Sets physical file, where resource is stored
        \param[in] file a file data
     */
    virtual void setPhysicalFile(sad::resource::ResourceFile * file);
    /*! Returns a physical fle, where resource supposedly stored
        \return file
     */
    sad::resource::ResourceFile * file() const;
    /*! Sets factory name, via which resource was created in factory
        \param[in] name a factory name
     */
    void setFactoryName(const sad::String & name);
    /*! Returns a factory name, via which resource was created in factory
        \return factory name
     */
    const sad::String & factoryName() const;
    /*! Returns an options, which resource was created with
        \return options
     */
    const picojson::value & options() const;
    /*! Returns whether resource supports loading from archive
        \return default implementation return false
     */
    virtual bool supportsLoadingFromTar7z() const;
protected: 
    /*! Loads a resource from specified file, using specified renderer for resolving some 
        properties.
        \param[in] file a file, via which a resource should be loaded
        \param[in] r  a renderer, which resource should be linked to (nullptr if global renderer)
        \param[in] options  an options for loading a resource
        \return whether loading was successfull
     */
    virtual bool load(
        const sad::resource::ResourceFile & file,
        sad::Renderer * r,
        const picojson::value& options
    ) = 0;
    /*! A parent folder of resoruce
     */
    sad::resource::Folder* m_folder;
    /*! A physical file, where resouces are stored
     */
    sad::resource::ResourceFile * m_file;
    /*! Whether we should store links to resources
     */
    bool m_store_links;
    /*! Returns name for resource
     */
    sad::String m_name;
    /*! A name for type, via which it was created in factory
     */
    sad::String m_factory_name;
    /*! A links to a resource
     */
    sad::Vector<sad::resource::AbstractLink*> m_links;
    /*! An options,  resource was created with
     */
    picojson::value m_options;
};

}

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::resource::Resource)

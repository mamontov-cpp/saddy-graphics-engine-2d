/*! \file resource/resourcefactory.h
    
    
    Contains definition of class ResourceFactory.

    Note, that resource factory can create resources by their type string.
 */
#pragma once
#include "resource.h"
#include "../sadptrhash.h"
#include "../sadstring.h"

namespace sad
{

namespace db
{
class StoredPropertyFactory;
}

namespace resource
{
/*! A special kind of delegate, which is used by Factory to create resources
 */
class Creator
{
public:
    /*! Creates a resource
        \return created resource (nullptr if cannot)
     */ 
    virtual sad::resource::Resource* create() = 0;
    /*! A creator must be inherited to create some stuff
     */
    virtual ~Creator();
};

/*! A special kind of delegate, which is used by Factory to create resources of specified type
 */
template<
typename _Resource
>
class CreatorFor: public resource::Creator
{
public:
    /*! Creates a resource
        \return created resource (nullptr if cannot)
     */ 
    virtual sad::resource::Resource* create()
    {
        return new _Resource();
    }
    /*! A creator must be inherited to create some stuff
     */
    virtual ~CreatorFor()
    {
        
    }
};

/*! A special kind of delegate, which is used by Factory to create files for specified type
 */
class FileCreator
{
public:
    /*! Creates a resource
        \return created resource (nullptr if cannot)
     */ 
    virtual sad::resource::ResourceFile* create() = 0;
    /*! A creator must be inherited to create some stuff
     */
    virtual ~FileCreator();
};

/*! A special kind of delegate, which is used by Factory to create files for specified type
 */
template<
typename _File
>
class FileCreatorFor: public resource::FileCreator
{
public:
    /*! Creates a resource
        \return created resource (nullptr if cannot)
     */ 
    virtual sad::resource::ResourceFile* create()
    {
        return new _File();
    }
    /*! A creator must be inherited to create some stuff
     */
    virtual ~FileCreatorFor()
    {
        
    }
};

/*! \class Factory

    Note, that resource factory can create resources by their type string.
 */
class Factory  
{	
public:	
    /*! Creates a factory
     */
    Factory();
    /*! Registers new resource in factory
     */ 
    template<typename T>
    void registerResource()
    {
        add(T::globalMetaData()->name(), 
            new resource::CreatorFor<T>());
    }
    /*! Registers file type to be used with this resource
     */
    template<typename _ResourceType, typename _FileType>
    void registerFileTypeFor()
    {
        sad::String tname = _ResourceType::globalMetaData()->name();
        if (m_file_creators.contains(tname))
        {
            delete m_file_creators[tname]; //-V515
        }
        m_file_creators.insert(tname, new resource::FileCreatorFor<_FileType>());
    }
    /*! Registers default file type to be used with this resource
     */
    template<typename _ResourceType>
    void registerDefaultFileTypeFor()
    {
        registerFileTypeFor<_ResourceType, sad::resource::ResourceFile>();
    }
    /*! This class can be inherited 
     */
    virtual ~Factory();
    /*! Adds new creator to a factory
        \param[in] name a name for factory
        \param[in] c a creator for it
     */
    virtual void add(const sad::String & name, resource::Creator * c);
    /*! Produces a resource by a string (nullptr if cannot)
        \param[in] name a resource name
        \return created resource
     */
    virtual sad::resource::Resource* create(const sad::String& name);
    /*! Creates a file type by a hint, using data
        \param[in] typehint type data
        \return data for a file
     */  
    virtual sad::resource::ResourceFile * fileByType(const sad::String & typehint);
    /*! Sets stored property factory for created schemas
        \param[in] factory  a factory for schema
     */
    virtual void setStoredPropertyFactory(sad::db::StoredPropertyFactory * factory);
    /*! Returns stored property factory
        \return a factory
     */
    virtual sad::db::StoredPropertyFactory* storedPropertyFactory() const;
protected:
    /*! Creates a hash for returning data
     */
    sad::PtrHash<sad::String, resource::Creator> m_creators;
    /*! A map for creating files
     */
    sad::PtrHash<sad::String, resource::FileCreator> m_file_creators;
    /*! A factory for stored properties. Used for creating itema in custom resources
     */
    sad::db::StoredPropertyFactory * m_factory;
private:
    /*! Disabled, factory is uncopyable
        \param[in] o other factory
     */
    Factory(const Factory & o);
    /*! Disabled, factory is uncopyable
        \param[in] o other factory
        \return self-reference
     */
    Factory & operator=(const Factory & o);
};

}

}

/*! \file resource/resourcefactory.h
	\author HiddenSeeker
	
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
		\return created resource (NULL if cannot)
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
		\return created resource (NULL if cannot)
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
	/*! This class can be inherited 
	 */
	virtual ~Factory();
	/*! Adds new creator to a factory
		\param[in] name a name for factory
		\param[in] c a creator for it
	 */
	virtual void add(const sad::String & name, resource::Creator * c);
	/*! Produces a resource by a string (NULL if cannot)
		\param[in] name a resource name
		\return created resource
	 */
	virtual sad::resource::Resource* create(const sad::String& name);
	/*! Creates a file type by a hint, using data
		\param[in] typehint type data
		\return data for a file
	 */  
	virtual sad::resource::PhysicalFile * fileByType(const sad::String & typehint);
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

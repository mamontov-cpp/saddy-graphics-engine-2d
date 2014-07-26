/*! \file dbstoredpropertyfactory.h
	\author HiddenSeeker

	Describes a factory for building stored properties.
 */
#pragma once
#include "dbstoredproperty.h"
#include "../sadptrhash.h"

namespace sad
{
	
namespace db
{

/*! A class, which is used to create stored properties
 */
class StoredPropertyFactory
{
public:

/*! Creates a stored property
 */
class AbstractDelegate
{
public:
	/*! Creates a property
		\return property
	 */
	virtual sad::db::Property* create() = 0;
	/*! Can be inherited
	 */
	virtual ~AbstractDelegate();
};

/*! Creates a stored property of specified type
 */
template<
	typename T
>
class Delegate: public sad::db::StoredPropertyFactory::AbstractDelegate
{
public:
	/*! This class could be instantiated
	 */
	Delegate()
	{
		
	}
	/*! Creates a property
		\return property
	 */
	virtual sad::db::Property* create()
	{
		return new sad::db::StoredProperty<T>();
	}
	/*! Can be inherited
	 */
	virtual ~Delegate()
	{
		
	}
};
	/*! A default factory
	 */
	StoredPropertyFactory();
	/*! This class could be inherited
	 */
	virtual ~StoredPropertyFactory();
	/*! Adds new stored property factory delegate
		\param[in] name a name of class of property type
		\param[in] d a delegate for creating property of type
	 */
	void add(const sad::String & name, sad::db::StoredPropertyFactory::AbstractDelegate * d);
	/*! Adds new stored property factory delegate of specified type
		\param[in] name a name of class of property type
	 */
	template<
		typename T
	>
	void add(const sad::String & name)
	{
		add(name, new sad::db::StoredPropertyFactory::Delegate<T>());
	}
	/*! Creates a property by it's class name
		\param[in] name a name value
		\return  property
	 */
	virtual sad::db::Property * create(const sad::String & name) const;
protected:
	/*! A delegate list
	 */
	sad::PtrHash<sad::String, sad::db::StoredPropertyFactory::AbstractDelegate> m_delegates;
};

}

}

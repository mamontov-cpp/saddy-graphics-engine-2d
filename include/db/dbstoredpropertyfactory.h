/*! \file dbstoredpropertyfactory.h
    

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
    /*! Clones a delegate
        \return new delegate copy
     */
    virtual sad::db::StoredPropertyFactory::AbstractDelegate* clone() = 0;
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
	Delegate() = default;
    /*! Creates a property
        \return property
     */
    virtual sad::db::Property* create() override
    {
        return new sad::db::StoredProperty<T>();
    }
    /*! Clones a delegate
        \return new delegate copy
     */
    virtual sad::db::StoredPropertyFactory::AbstractDelegate* clone() override
    {
        return new sad::db::StoredPropertyFactory::Delegate<T>();
    }
    /*! Can be inherited
     */
	virtual ~Delegate()  override = default;
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
    /*! Creates a property by it's type of value
        \param[in] name a name of property's type of value
        \return  property or nullptr if unable to created
     */
    virtual sad::db::Property * create(const sad::String & name) const;
    /*! Whether factory can create property by it's type of value
        \param[in] name a name property's type of value
        \return property type name
     */
    virtual bool canCreate(const sad::String & name) const;
    /*! Clones a factory
        \return new factory
     */
    virtual sad::db::StoredPropertyFactory* clone() const;
protected:
    /*! A delegate list
     */
    sad::PtrHash<sad::String, sad::db::StoredPropertyFactory::AbstractDelegate> m_delegates;
};

}

}

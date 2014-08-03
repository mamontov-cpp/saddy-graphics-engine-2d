/*! \file dbobjectfactory.h
	\author HiddenSeeker
	
	A factory for creating objects, by their names. Could be used for deserializing  
	objects from file in database.
 */ 
#pragma once
#include "dbobject.h"
#include "schema/schema.h"


namespace sad
{

namespace db
{
/*! Describes a factory for creating objects by their name. Used for deserializing 
    objects in database
 */
class ObjectFactory
{
public:

	/*! An abstract delegate for creating items of factory
	 */
	class AbstractDelegate
	{
	public:
		/*! Creates new object
			\return object
		 */
		virtual sad::db::Object * create() = 0;
		/*! This class can be inherited
		 */
		virtual ~AbstractDelegate();
	};
	
	/*!  A delegate for creating object by invoking it's constructor
	 */
	template<
		typename T
	>
	class Delegate: public sad::db::ObjectFactory::AbstractDelegate
	{
	public:
		/*! A default delegate for object
		 */
		inline Delegate()
		{
		
		}
		/*! Creates new object
			\return object
		 */
		virtual sad::db::Object * create()
		{
			return new T();
		}
		/*! This class can be inherited
		 */
		virtual ~Delegate()
		{
		
		}
	};
	/*! Creates new factory with default objects
	 */
	ObjectFactory();
	
	/*! Adds new metadata to factory
		\param[in] name a class name forobject
		\param[in] schema a pointer to schema
		\param[in] own  whether factory owns schema
		\param[in] d delegate for creating objects
	 */
	virtual void add(
		const sad::String & name,
		sad::db::schema::Schema* schema, 
		bool own, 
		sad::db::ObjectFactory::AbstractDelegate * d
	);
	/*! Adds new metadata to factory
		\param[in] name a class name forobject
		\param[in] schema a pointer to schema
		\param[in] own  whether factory owns schema
	 */	
	template<
		typename T
	>
	void add(
		const sad::String & name,
		sad::db::schema::Schema* schema, 
		bool own = true
	)
	{
		this->add(name, schema, own, new sad::db::ObjectFactory::Delegate<T>());
	}
	/*! Returns schema for an object's class
		\param[in] name  a name of object's class
		\return schema or NULL, if no schema specified
	 */
	virtual sad::db::schema::Schema* schema(const sad::String& name);
	/*! Creates new object, specified by class name
		\param[in] name a name of object's class
		\return object or NULL, if can't create object 
	 */
	virtual sad::db::Object* create(const sad::String& name);

	/*! This class can be inherited
	 */
	virtual ~ObjectFactory();
protected:
	/*! An entry, stored in factory, holding sources to all data
	 */
	struct Entry
	{
		/*! Whether entry owns schema and should kill it, when dying
		 */
		bool OwnsSchema;
		/*! A schema inside of entry
		 */
		sad::db::schema::Schema* Schema;
		/*! A delegate, which could create objects
		 */
		sad::db::ObjectFactory::AbstractDelegate* Delegate;
		
		/*! Constructs new entry, from following data
			\param[in] own whether entry owns schema
			\param[in] schema a schema, which is being owned
			\param[in] d a delegate, which could create object
		 */
		inline Entry(bool own, sad::db::schema::Schema* schema, sad::db::ObjectFactory::AbstractDelegate* d)
		: OwnsSchema(own), Schema(schema), Delegate(d)
		{
		
		}
		/*! Destroys delegate and schema, if needs to
		 */
		~Entry();
	};
	
	/*! Contains all metadata with class name as a key
	 */
	sad::PtrHash<sad::String, sad::db::ObjectFactory::Entry> m_metadata_container;
};

}

}

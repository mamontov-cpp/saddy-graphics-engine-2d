/*! \file serializableobject.h
	\author HiddenSeeker

	A serializable object  that can be easily serialized to stream data
 */
#include "property.h"
#include <sadhash.h>
#include "../object.h"
#pragma once

/*! A serialization entry , that contents a string representation of object
 */
class SerializationEntry
{
 public:
		 /*! Name of object
		  */
		 sad::String Name;
		 /*! Vector of properties names
		  */
		 sad::Vector<sad::String> PropertiesName;
		 /*! Vector of properties values data
		  */
		 sad::Vector<sad::String> PropertiesValue;


		 /*! An empty entry
		  */
		 inline SerializationEntry() {}
};


class SerializableContainer;
/*! A object, that can be easily serialized and deserialized
 */
class SerializableObject: public sad::Object
{
 SAD_OBJECT
 private:
		 sad::Hash<sad::String, AbstractProperty *> m_properties; //!< Properties a data
		 SerializableContainer * m_parent; //!< Parent container
 public:
		/*! Parent container
			\return parent container
		 */
		inline SerializableContainer * parent() const { return m_parent;}
		/*! Sets a parent
			\param[in] parent parent object
		 */
		inline void setParent(SerializableContainer * parent) { m_parent = parent; }
	    /*! Adds new property to an object
			\param[in] name name of properties
			\param[in] prop a property
		 */ 
	    void addProperty(const sad::String & name, AbstractProperty * prop);
		/*! Returns a property if it exists, otherwise null
			\param[in] name name of property
			\return property
		 */
		AbstractProperty * getProperty(const sad::String & name);

		/*! Saves an object to a single entry
			\return new object
		 */
		virtual SerializationEntry * save();

		/*! Loads an object from entry
			\param[in] entry entry
		 */
		virtual void load(SerializationEntry * entry);


		/*! Resolves deferred links when loaded
		 */
		virtual void resolveDeferred();
		/*! Returns a string type
			\return a string type of object
		 */
		virtual sad::String type();
		/*! Frees a memory from properties data
		 */
		virtual ~SerializableObject();
};

/*! A factory used for serializable objects creation
 */
class SerializableFactory
{
 public:
	     /*! Produces an object by name
			 \param[in] obj object
			 \return data
		  */
		 virtual SerializableObject* produce(const sad::String & obj) ;
		 /*! Does nothing
		  */
		 ~SerializableFactory();
};

/*! \file serializableobject.h
	\author HiddenSeeker

	A serializable object  that can be easily serialized to stream data
 */
#include "property.h"
#include <templates/hhash.hpp>
#pragma once

/*! A serialization entry , that contents a string representation of object
 */
class SerializationEntry
{
 public:
		 /*! Name of object
		  */
		 hst::string Name;
		 /*! Vector of properties names
		  */
		 hst::vector<hst::string> PropertiesName;
		 /*! Vector of properties values data
		  */
		 hst::vector<hst::string> PropertiesValue;


		 /*! An empty entry
		  */
		 inline SerializationEntry() {}
};


class SerializableContainer;
/*! A object, that can be easily serialized and deserialized
 */
class SerializableObject
{
 private:
		 hst::hash<hst::string, AbstractProperty *> m_properties; //!< Properties a data
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
	    void addProperty(const hst::string & name, AbstractProperty * prop);
		/*! Returns a property if it exists, otherwise null
			\param[in] name name of property
			\return property
		 */
		AbstractProperty * getProperty(const hst::string & name);

		/*! Saves an object to a single entry
			\param[in] context context
			\return new object
		 */
		virtual SerializationEntry * save(ActionContext * context);

		/*! Loads an object from entry
			\param[in] entry entry
			\param[in] context context
		 */
		virtual void load(SerializationEntry * entry,ActionContext * context);


		/*! Resolves deferred links when loaded
		 */
		virtual void resolveDeferred(ActionContext * context);
		/*! Returns a string type
			\return a string type of object
		 */
		virtual hst::string type()=0;
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
		 virtual SerializableObject* produce(const hst::string & obj) ;
		 /*! Does nothing
		  */
		 ~SerializableFactory();
};

/*! \file   abstractproperty.h
	\author HiddenSeeker

	Here defined a property that can be easily serialized
 */
#include "variant.h"
#pragma once


class SerializableObject;

/*! \class AbstractProperty

	An abstract propety marker
 */
class AbstractProperty
{   
 private:
	   SerializableObject * m_parent; //!< A parent object for editor
 protected:
	   inline AbstractProperty() { m_parent = NULL;}
 public:
		/*! Sets a parent serializable object
			\param[in] obj serializable object
		 */
		void setParent(SerializableObject * obj)
		{ m_parent = obj; }
		/*! Returns a parent serializable object
			\return serializable object
		 */
		SerializableObject * getParent()
		{ return m_parent; }

		/*! Calls a property, if this is method
		 */
		virtual void call(const sad::VariantVector & args,ActionContext * context);
		/*! Sets a property value
		 */
		virtual void set(const sad::Variant & value,ActionContext * context);
		/*! Returns a property value
		 */
		virtual sad::Variant * get(ActionContext * context) const;
		/*! Returns a type of property
		 */
		inline hst::string type(ActionContext * context) const 
		{ 
			sad::Variant * tmp = get(context);
			hst::string result = tmp->type();
			delete tmp;
			return result; 
		}
			    
	    /*! Returns a string representation of property
			\return string representation
		 */
	    virtual hst::string save(ActionContext * context) const;
	    
        /*! Loads data from string
			\param[in] str      string data
			\param[in] context  action context
		 */
	    virtual void load(const hst::string & str, ActionContext * context);

		/*! Whether we should load this property. Methods won't be saveable
		 */
		virtual bool saveable() const;
        /*! Whether this is method
		 */
		virtual bool callable() const;

		/*! This function is called, when all props a set and all objects are created to ensure
		    that pointers are resolved correctly
		 */
		virtual void resolveDeferred(ActionContext * context);

		virtual ~AbstractProperty();
};

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
		SerializableObject * getParent() const
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
		inline sad::String type(ActionContext * context) const 
		{ 
			sad::Variant * tmp = get(context);
			sad::String result = tmp->type();
			delete tmp;
			return result; 
		}
			    
	    /*! Returns a string representation of property
			\return string representation
		 */
	    virtual sad::String save(ActionContext * context) const;
	    
        /*! Loads data from string
			\param[in] str      string data
			\param[in] context  action context
		 */
	    virtual void load(const sad::String & str, ActionContext * context);

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

		/*! Determines a return value for method
		 */
		virtual sad::Variant * returnValue() const; 
		/*! Determines, whether method returns a value
		 */
		virtual bool returnsValue() const;
};

/*! \file   abstractproperty.h
	\author HiddenSeeker

	Here defined a property that can be easily serialized
 */
#pragma once
#include "variant.h"

#include "../3rdparty/format/format.h"

#include <stdexcept>


class SerializableObject;

/*! \class AbstractProperty

	An abstract propety marker
 */
class AbstractProperty
{   
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
		virtual void call(const sad::VariantVector & args);
		/*! Sets a property value
		 */
		virtual void set(const sad::Variant & value);
		/*! Returns a property value
		 */
		virtual sad::Variant * get() const;
		/*! Returns a type of property
			\return type of property, NULL if nothing returned
		 */
		inline sad::String type() const 
		{ 
			sad::Variant * tmp = get();
			sad::String result = "NULL";
			if (tmp != NULL)
			{
				sad::String result = tmp->type();
				delete tmp;
			} 
			return result; 
		}
			    
	    /*! Returns a string representation of property
			\return string representation
		 */
	    virtual sad::String save() const;
	    
        /*! Loads data from string
			\param[in] str      string data
		 */
	    virtual void load(const sad::String & str);

		/*! Whether we should load this property. Methods won't be saveable
		 */
		virtual bool saveable() const;
        /*! Whether this is method
		 */
		virtual bool callable() const;

		/*! This function is called, when all props a set and all objects are created to ensure
		    that pointers are resolved correctly
		 */
		virtual void resolveDeferred();

		virtual ~AbstractProperty();

		/*! Determines a return value for method
		 */
		virtual sad::Variant * returnValue() const; 
		/*! Determines, whether method returns a value
		 */
		virtual bool returnsValue() const;
protected:
	/*! We disallow creation of property with no parent. You can store property
		by reference, not by a value
	 */
	inline AbstractProperty() 
	{ 
		m_parent = NULL;
	}
	/*! Throws std::invalid_argument exception, when supplied more or less 
		arguments to called method
		\param[in] needed how many arguments method needs, to be called
		\param[in] given  how many arguments was given to method, when it's called
		\throw std::invalid_argument
	 */
	inline void throwInvalidArgument(int needed, int given)
	{
		std::string message = str(
			fmt::Format("Method call requires {0} arguments, {1} given")
			<< needed
			<< given
		);
		throw std::invalid_argument(message);
	}
private:
	SerializableObject * m_parent; //!< A parent object for a property
};

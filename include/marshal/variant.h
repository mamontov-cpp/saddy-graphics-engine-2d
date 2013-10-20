/*! \file variant.h
	\author HiddenSeeker

	Here described a variant type, used by properties to set or get values
 */
#pragma once
#include "typestrings.h"

#include "../3rdparty/format/format.h"

#include <string.h>
#include <stdio.h>
#include <stdexcept>


namespace sad
{

namespace misc
{
/*! Miscellaneous free function
 */
template<
typename T
>    
void variant_cleanup(void * d)  { delete reinterpret_cast<T*>(d); }


}
/*! Describes a variant class, used for setting or getting data
 */
class Variant
{
public:
	/*! Constructs a new variant from specified object.
		For creating object, you must specify it's type name, via 
		DEFINE_PROPERTY_TYPESTRING
		\param[in] object object data
	*/
	template<
		typename T
	>
	Variant( const T & object)
	{
		m_object = new T(object);
		m_destructor = sad::Variant::free<T>;
		m_type = abstract_names::type_string<T>::type();
	};

	/*! Returns held object, if object type inside matches specified type, otherwise
		throws std::logic_error
		\return held object
	 */
	template<
		typename T
	>
	const T & get() const 
	{  
		if (hasType<T>())
		{
			return *reinterpret_cast<T*>(m_object);
		}
		
		// Throw exception, if types do not match
		sad::String type_to_be_casted = abstract_names::type_string<T>::type();
		std::string message = str(
			fmt::Format("Can\'t cast type \"{0}\" to type \"{1}\"")
			<< m_type
			<< type_to_be_casted
		);
		throw std::logic_error(message);
		
		// Create default object to make project compilable
		// This line will be never called
		T * default_data = NULL;
		return *(default_data);
	}

	/*! Returns a type of variant
		\return type of variant
	 */
	inline const sad::String & type() const 
	{
		return m_type; 
	}

	/*! Determines, whether variant has a type, declared in a type, declared in
		a variant
		\return whether varian has a type
	 */
	template<
		typename _Type
	>
	inline bool hasType() const 
	{ 
		return abstract_names::type_string<_Type>::type() == m_type; 
	}
	/*! Destructor  destroys object only 
		if destructor callback set 
		to prevent unnecessary destructions
	 */
	inline ~Variant()  
	{  
		if (m_destructor) 
		{
			m_destructor(m_object); 
		}
	}
private:
	/*! Pointer toa real object, which is being hold inside
	 */
	void *  m_object;	
	/*! Destructor function, which frees a memory, upon destruction of object
	 */
	void (*m_destructor)(void *); 
	/*! Name of type of object, which is being hold inside
	 */
	sad::String m_type;
	/*! Destroys an object, calling destructor and freeing memory of object.
		Used as polymorphic destructor
		\param[in] p pointer to memory
	 */
	template<
		typename _Type
	>
	static void free(void * p)
	{
		delete reinterpret_cast<_Type *>(p);
	}
};


/*! This vector holds all new variants and destroys them
 */
class VariantVector: public sad::Vector<sad::Variant *>
{
 public:
		/*! Destroys all hold vectors of variants
		 */
		~VariantVector();
};

}



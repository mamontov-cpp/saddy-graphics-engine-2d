/*! \file db/variant.h
	\author HiddenSeeker
	
	Contains definition of class Variant, which could be used to
	box values of various types and work with them. 
	
	Note, that this is abstraction for value of property, not the property itself.
 */
#pragma once
#include "../sadstring.h"
#include "dbtypename.h"
#include "save.h"
#include "load.h"

namespace db
{

namespace variant
{

/*! Copies a variant value
	\param[in] o a value
	\return value
 */
template<typename T>
void * copy_value(void * o)
{
	return new T(*(reinterpret_cast<T*>(o)));		
}

/*! Deletes a variant value
	\param[in] o a value
	\return value
 */
template<typename T>
void delete_value(void * o)
{
	delete reinterpret_cast<T*>(o);		
}

}

/*! \class Variant

	Could be used to box values of various types and work with them. 
	Note, that this is abstraction for value of property, not the property itself.
 */
class Variant  
{	
protected:
	/*! A boxed object in variant
	 */
	void * m_object;
	/*! Whether value of variant is sad object
	 */
	bool is_sad_object;
	/*! A name of type, boxed in variant
	 */
	sad::String  m_typename;
	/*! A function for copying a value
	 */
	void * (*m_copy)(void *);
	/*! A function for deleting a value
	 */
	void (*m_delete)(void *);
public:	
	/*! Construct an empty variant value
	 */
	Variant();
	/*! A constructor, which assigns a value to a variant
	 */
	template<typename T>
	Variant(const T & v);
	/*! Frees a value from variant
	 */
	virtual ~Variant();
};

}

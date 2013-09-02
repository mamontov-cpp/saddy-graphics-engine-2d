/*! \file variant.h
	\author HiddenSeeker

	Here described a variant type, used by properties to set or get values
 */
#include "typestrings.h"
#include "exceptions.h"
#include <string.h>
#include <stdio.h>
#pragma once


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
 private:
		  void *  m_object;                //!< Pointer to a real object, that holded inside
		  /*! Destructor function, which frees a memory, upon destruction of object
		   */
		  void (*m_destructor)(void *); 
		  hst::string m_type;              //!< Type string of object

		  /*! Returns a variant::get function context
		   */
		  inline hst::string getGetContext() const 
		  {
			  char data[70];
			  sprintf(data,"Performing %lx->sad::variant::get.",reinterpret_cast<unsigned long>(this));
			  return hst::string(data);
		  }
 public:
		   /*! Constructs an object from value
			   \param[in] object object data
		    */
		   template<typename T>
		   Variant( const T & object)
		   {
			   m_object = new T(object);
			   m_destructor = sad::misc::variant_cleanup<T>;
			   m_type = abstract_names::type_string<T>::type();
		   };

		   /*! Returns a real object, if possible
			   \param[in] cont context
		    */
		   template<typename T>
		   const T & get(ActionContext * cont) const 
		   {  
			  if (hasType<T>())
			  {
				  return *reinterpret_cast<T*>(m_object);
			  }
		      //Push new action
			  cont->pushAction(getGetContext());
			  //Create new exception
			  hst::string cast_type = abstract_names::type_string<T>::type();
			  serializable::InvalidPropertyType * exc = new serializable::InvalidPropertyType(cast_type,
				                                                                              m_type,
		        																			  cont);
			  cont->popAction();
			  throw exc;

			  return *(new T());
		   }

		   /*! Returns a type of variant
		    */
		   inline const hst::string & type() {return m_type; }
		   /*! Determines, whether type of object is declared in template
		    */
		   template<typename T>
		   inline bool hasType() const { return abstract_names::type_string<T>::type() == m_type; }
		   /*! Destructor  destroys object only if destructor set to prevent unnecessary destructions
		    */
	       inline ~Variant()  {  if (m_destructor) m_destructor(m_object); }
};


/*! This vector holds all new variants and destroys them
 */
class VariantVector: public hst::vector<sad::Variant *>
{
 public:
		/*! Destroys all hold vectors of variants
		 */
		~VariantVector();
};

}



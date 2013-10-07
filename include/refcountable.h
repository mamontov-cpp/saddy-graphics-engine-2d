/*! \file   refcountable.h
	\author HiddenSeeker
	Declares a simple reference-countable object.
*/
#pragma once
#include <new>

namespace sad
{

class RefCountable
{
 private:
	      /*! References to object
		  */
	      int m_references;
 public:
	     inline RefCountable() { m_references=0; }
		 inline void addRef()  
		 {
			 ++m_references;
		 }
		 inline void delRef()  
		 {
			 --m_references; 
			 if (m_references==0) delete this; 
		 }
		 virtual ~RefCountable();
};

}

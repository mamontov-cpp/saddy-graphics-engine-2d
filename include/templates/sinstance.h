/*! \file sinstance.h
	\author HiddenSeeker
	\brief  Declares a template class for unmanaged static instance of object
*/
#include <new>
#pragma once
/*! Class of static instance of objects
*/
template<typename _Class>
class Instance
{
private:
    /*! An instance of class
    */
    static _Class * m_instance;
public:
	/*! Declares an instance
		\return instance
	*/
	static inline _Class * ref() {return m_instance; }
	/*! Sets an instance for data
		\param[in] p new instance
	*/
	static void set(_Class * p) {m_instance=p;}
	/*! Nullifies an instance
	*/
	static void zero() {m_instance=NULL;}
};


//======= Source code goes here ========
template<typename _Class>
_Class * Instance<_Class>::m_instance=NULL;

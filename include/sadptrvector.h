/*! \file    sadptrvector.h
    \author  mamontov-cpp

	This file contains a definition of vector, which holds a pointers and destroys them
	automatically, when destroyed.
*/
#pragma once
#include "sadvector.h"

namespace sad
{

/*! A class for holding pointers by value, that they reference. 
	Automatically cleans memory, when destroyed
 */
template<typename T>
class PtrVector: public sad::Vector<T*>
{
 public:
	/*! Creates new empty vector, holding a pointers
	 */
	inline PtrVector() : sad::Vector<T*>() 
	{
	}
	/*! Frees memory for each contained element
	 */
	virtual ~PtrVector() 
	{
		for(size_t i = 0; i < this->size(); i++)
			delete (*this)[i];
	}
};

}

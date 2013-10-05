/*! \file    ptrvector.h
    \author  HiddenSeeker

	This file contains a definition of vector, which holds a pointers and destroys them
	automatically, when destroyed.
*/
#pragma once
#include "hlvector.h"

namespace sad
{

/*! A class for owning pointers. Automatically cleans memory, when destroyed
 */
template<typename T>
class ptrvector: public sad::vector<T*>
{
 public:
	/*! Creates new empty vector, holding a pointers
	 */
	inline ptrvector() : sad::vector() 
	{
	}
	/*! Frees memory for each contained element
	 */
	~ptrvector() 
	{
		for(size_t i = 0; i < size(); i++)
			delete (*this)[i];
	}
};

}

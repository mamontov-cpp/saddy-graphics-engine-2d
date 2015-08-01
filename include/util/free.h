/*! \file free.h
	

	Contains definition for free, which can free a resources, stored in vectors
 */
#pragma once
#include "../sadvector.h"

namespace sad
{

namespace util
{


/*! Frees resources, stored in vector
	\param[in] v a vector, whose resources must be freed
 */ 
template<typename T>
void free(const sad::Vector<T*> & v)
{
	for(size_t i = 0; i < v.size(); i++)
	{
		delete v[i];
	}
}

}

}

/*! \file    ptrhash.hpp
    \author  HiddenSeeker
	\brief   Contains hash implementation with pointer function

	Contains a simple hash, with pointers, which is freed automatically
*/
#include "hhash.hpp"
#pragma once

namespace hst
{

/*! A container, which stores a pointers to objects and frees them, when
    destroyed
 */
template<typename Key, typename T>
class ptrhash: public hst::hash<Key, T *>
{
 public:
	 /*! Frees a memory, consumed by pointers
	  */
	 ~ptrhash()
	 {
		typename hst::ptrhash<Key, T>::iterator it = this->begin();
		for(it; it != this->end(); it++)
		{
			delete it.value();
		}
	 }
};

}
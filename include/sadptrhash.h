/*! \file    sadptrhash.h
    \author  mamontov-cpp
    \brief   Contains hash implementation with pointer function

    Contains a simple hash, with pointers, which is freed automatically
*/
#pragma once
#include "sadhash.h"

namespace sad
{

/*! A container, which stores a pointers to objects and frees them, when
    destroyed
 */
template<typename Key, typename T>
class PtrHash: public sad::Hash<Key, T *>
{
 public:
     /*! Frees a memory, consumed by pointers
      */
     ~PtrHash()
     {
        typename sad::PtrHash<Key, T>::iterator it = this->begin();
        for(; it != this->end(); ++it)
        {
            delete it.value();
        }
     }
};

}
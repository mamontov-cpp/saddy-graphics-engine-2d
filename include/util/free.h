/*! \file free.h
    

    Contains definition for free, which can free a resources, stored in vectors
 */
#pragma once
#include "../sadvector.h"
#include "../sadhash.h"

namespace sad
{

namespace util
{


/*! Frees resources, stored in vector
    \param[in] v a vector, whose resources must be freed
 */ 
template<typename T>
void free(const sad::Vector<T*>& v)
{
    for(size_t i = 0; i < v.size(); i++)
    {
        delete v[i];
    }
}

/*! Frees resources, stored in hash
    \param[in] v a hash, whose values must be freed
 */
template<typename K, typename V>
void free_values(const sad::Hash<K, V>& v)
{
    for (typename sad::Hash<K, V>::const_iterator it = v.const_begin(); it != v.const_end(); ++it)
    {
        delete (it->second);
    }
}

}

}

/*! \file    include/sadhash.h
    \brief   Contains basic hash-table dictionary definition

    Contains a simple hashtable, based on ElfHash function
*/
#pragma once
#include <unordered_map>
#include <functional>

#include "sadstring.h"
#include "sadwstring.h"
#include "sadpair.h"

namespace std
{

/*! Hash value for string
 */
template<>
struct hash<sad::String>
{
/*! Returns hash value for string
    \param[in] o string
    \return value
 */
size_t operator()(const sad::String& o) const;
};

/*! Hash value for wide string
 */
template<>
struct hash<sad::WString>
{
/*! Returns hash value for wide string
    \param[in] o string
    \return value
 */
size_t operator()(const sad::WString& o) const;
};

}

namespace sad
{
    
/*! \class Hash
    Class of a simple hash-based dictionary, based on std::unordered_map
*/
template<
    typename Key, 
    typename T
>
class Hash : public std::unordered_map<Key, T>
{
public:
    /*! Constant iterator for hash
     */
    class const_iterator: public std::unordered_map<Key, T>::const_iterator
    {
    public:
        /*! Constructs broken iterator
         */
        const_iterator()
        {
            
        }
        /*! A copy constructor for iterator
            \param[in] it other iterator
         */
        const_iterator(const typename std::unordered_map<Key, T>::const_iterator& it)
        : std::unordered_map<Key, T>::const_iterator(it)
        {
            
        }
        /*! Destructor
         */
        ~const_iterator()
        {
            
        }
        /*! Returns a key for iterator
            \return a key value for iterator
         */
        const Key& key() const
        {
            return (*this)->first;
        }
        /*! Returns a value for iterator
            \return a value for iterator
         */ 
        const T&  value() const
        {
            return (*this)->second;
        }
    };
    /*! A common iterator for hash
     */
    class iterator: public std::unordered_map<Key, T>::iterator
    {
    public:
        /*! Constructs broken iterator
         */
        iterator()
        {
            
        }
        /*! A copy constructor for iterator
            \param[in] it other iterator
         */
        iterator(const typename std::unordered_map<Key, T>::iterator& it)
        : std::unordered_map<Key, T>::iterator(it)
        {
            
        }
        /*! Destructor
         */
        ~iterator()
        {
            
        }
        /*! Returns a key for iterator
            \return a key value for iterator
         */
        const Key& key() const
        {
            return (*this)->first;
        }
        /*! Returns a value for iterator
            \return a value for iterator
         */ 
        T &  value() const
        {
            return (*this)->second;
        }
    };
       
    /*! Returns a begin iterator
        \return an iterator for starting element
     */
    iterator begin()
    {
        return iterator(this->std::unordered_map<Key, T>::begin());
    }
    /*! Returns an end iterator
        \return an iterator for ending element
    */
    iterator end()
    {
        return iterator(this->std::unordered_map<Key, T>::end());
    }

    /*! Returns a begin iterator
        \return  an iterator for stating element
     */
    const_iterator const_begin() const
    {
        return const_iterator(this->std::unordered_map<Key, T>::begin());
    }
    /*! Returns an end iterator
        \return an iterator for ending element
     */
    const_iterator const_end() const
    {
        return const_iterator(this->std::unordered_map<Key, T>::end());
    }

    /*! Init an empty hash
     */
    Hash() //-V730
    {
        
    }
    /*! Creates a hash, with one key and one value
        \param[in] k1 first key
        \param[in] v1 first value
     */
    Hash(const Key & k1, const T & v1)
    {
        this->insert(std::make_pair(k1, v1));
    }
    /*! Creates a hash, with two keys and two values. 
        \param[in] k1 first key
        \param[in] v1 first value
        \param[in] k2 second key
        \param[in] v2 second value
    */
    Hash(const Key & k1, const T & v1, const Key & k2, const T & v2) //-V730
    {
        this->insert(k1, v1);
        this->insert(k2, v2);
    }
    /*! Creates a hash, with three keys and three values. 
        \param[in] k1 first key
        \param[in] v1 first value
        \param[in] k2 second key
        \param[in] v2 second value
        \param[in] k3 third key
        \param[in] v3 third value
    */
    Hash(const Key & k1, const T & v1, const Key & k2, const T & v2, const Key & k3, const T & v3) //-V730
    {
        this->insert(k1, v1);
        this->insert(k2, v2);
        this->insert(k3, v3);
    }
    /*! Seeks a key-value pair in hash. If not found, the default value is
       returned. Note, that it could change contains of hash table, if element
       is absent, it would be inserted into container with default value.
       \param[in] k key
       \return value
    */
    const T & operator[](const Key & k) const
    {
        try {
            return this->at(k);
        } catch(...) {
            return m_default_value;
        }
        return m_default_value;
    }
    /*! Seeks a key-value pair in hash. If not found, the default value is
       returned. Note, that it could change contains of hash table, if element
       is absent, it would be inserted into container with default value.
        \param[in] k key
        \return value
     */
    T & operator[](const Key & k)
    {
        try {
            return this->at(k);
        } catch(...) {
            return m_default_value;
        }
        return m_default_value;
    }
    /*! Detects, whether exists some key
        \param[in] k key
        \return if value with key exists in table
     */
    bool contains(const Key & k) const
    {
        return this->find(k) != this->const_end();
    }
    /*! Appends a value to a table. If the key does not exists, replaces it.
        \param[in] k key;
        \param[in] v value
     */
    void insert(const Key & k, const T & v)
    {
        // std::unordered_map cannot replace the already existing pair, so we need to
        // do it ourselves. 
        if (this->find(k) != this->const_end())
        {
            this->erase(k);
        }
        this->std::unordered_map<Key, T>::insert(std::make_pair(k, v));
    }
    /*! Removes a value from a table
        \param[in] k key
     */
    void remove(const Key & k)
    {
        this->erase(k);
    }
    /*! Returns amount of elements in table
        \return amount of elements
     */
    unsigned long count() const
    {
        return this->size();
    }
protected:
    /*! Preserve default value, which will be returned in some cases
     */
    T m_default_value;
};

}
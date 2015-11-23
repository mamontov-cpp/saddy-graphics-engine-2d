/*! \file    sadhash.h
    \brief   Contains basic hash-table dictionary definition

    Contains a simple hashtable, based on ElfHash function
*/
#pragma once
#ifdef USE_OLD_SAD_HASH_IMPLEMENTATION
	#include "util/sadhash.h"
#else

#ifdef USE_EXTERNAL_BOOST 
#include <boost/functional/hash.hpp>
#include <boost/unordered/unordered_map.hpp>
#else
#include "3rdparty/boost-dist/boost/functional/hash/extensions.hpp"
#include "3rdparty/boost-dist/boost/functional/hash.hpp"
#include "3rdparty/boost-dist/boost/unordered/unordered_map.hpp"
#endif

#endif


#ifndef USE_OLD_SAD_HASH_IMPLEMENTATION

#include "sadstring.h"
#include "sadwstring.h"
#include "sadpair.h"

namespace sad
{

/*! Returns hash value for sad::String type
	\param[in] b value
 */
std::size_t hash_value(const sad::String& b);
/*! Returns hash value for sad::String type
	\param[in] b value
 */
std::size_t hash_value(const sad::WString& b);
	
/*! \class Hash
    Class of a simple hash-based dictionary, based on boost::unordered_map
*/
template<
    typename Key, 
    typename T
>
class Hash
{
    friend class const_iterator;
    friend class iterator;
public:
    /*! Constant iterator for hash
     */
    class const_iterator: public boost::unordered_map<Key, T>::const_iterator
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
		const_iterator(const typename boost::unordered_map<Key, T>::const_iterator& it)
		: boost::unordered_map<Key, T>::const_iterator(it)
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
    class iterator: public boost::unordered_map<Key, T>::iterator
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
		iterator(const typename boost::unordered_map<Key, T>::iterator& it)
		: boost::unordered_map<Key, T>::iterator(it)
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
	    return iterator(m_map.begin());
    }
    /*! Returns an end iterator
		\return an iterator for ending element
	*/
    iterator end()
    {
	    return iterator(m_map.end());
    }

    /*! Returns a begin iterator
		\return  an iterator for stating element
     */
    const_iterator const_begin() const
    {
	    return const_iterator(m_map.begin());
    }
    /*! Returns an end iterator
		\return an iterator for ending element
     */
    const_iterator const_end() const
    {
	    return const_iterator(m_map.end());
    }

    /*! Inits an empty hash
     */
    Hash()
    {
	    
    }
    /*! Creates a hash, with one key and one value
        \param[in] k1 first key
        \param[in] v1 first value
     */
    Hash(const Key & k1, const T & v1)
    {
        m_map.insert(std::make_pair(k1, v1));
    }
    /*! Creates a hash, with two keys and two values. 
        \param[in] k1 first key
        \param[in] v1 first value
        \param[in] k2 second key
        \param[in] v2 second value
    */
    Hash(const Key & k1, const T & v1, const Key & k2, const T & v2)
    {
        m_map.insert(std::make_pair(k1, v1));
        m_map.insert(std::make_pair(k2, v2));
    }
    /*! Creates a hash, with three keys and three values. 
        \param[in] k1 first key
        \param[in] v1 first value
        \param[in] k2 second key
        \param[in] v2 second value
        \param[in] k3 third key
        \param[in] v3 third value
    */
    Hash(const Key & k1, const T & v1, const Key & k2, const T & v2, const Key & k3, const T & v3)
    {
        m_map.insert(std::make_pair(k1, v1));
        m_map.insert(std::make_pair(k2, v2));
        m_map.insert(std::make_pair(k3, v3));
	}	
	/*! Clears a hash
     */
    void clear()
    {
	    m_map.clear();
    }
    /*! Seeks a key-value pair in hash. If not found, the default value is
       returned. Note, that it could change contains of hash table, if element
	   is absent, it would be inserted into container with default value.
       \param[in] k key
       \return value
    */
    const T & operator[](const Key & k) const
    {
	    return m_map.at(k);
    }
    /*! Seeks a key-value pair in hash. If not found, the default value is
       returned. Note, that it could change contains of hash table, if element
	   is absent, it would be inserted into container with default value.
        \param[in] k key
        \return value
     */
    T & operator[](const Key & k)
    {
	    return m_map.at(k);
    }
    /*! Detects, whether exists some key
        \param[in] k key
		\return if value with key exists in table
     */
    bool contains(const Key & k) const
    {
	    return m_map.find(k) != m_map.end();
    }
    /*! Appends a value to a table
        \param[in] k key;
        \param[in] v value
     */
    void insert(const Key & k, const T & v)
    {
	    m_map.insert(std::make_pair(k, v));
    }
    /*! Removes a value from a table
        \param[in] k key
     */
    void remove(const Key & k)
    {
	    m_map.erase(k);
    }
    /*! Returns amount of elements in table
		\return amount of elements
     */
    unsigned long count() const
    {
	    return m_map.size();
    }
private:
	/*! An inner unordered map, that is being wrapped
	 */
    boost::unordered_map<Key, T> m_map;
};

}

#endif
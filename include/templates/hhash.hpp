/*! \file    hhash.hpp
    \author  Various
	\brief   Contains basic hash-table dictionary definition

	Contains a simple hashtable, based on ElfHash function
*/
#include "hlvector.hpp"
#include "hdeque.hpp"
#include "hstring.h"
#include "hpair.hpp"
#pragma once

#ifndef __H_HASH_HPP
 #define __H_HASH_HPP
namespace hst
{
  /*! Hashes basically a 0-terminated string. Based on ElfHash
      \param[in] name name
	  \return hash
  */
  inline unsigned long basicHash(const unsigned char * name);
  /*! Hashes basically a 0-terminated string. Based on ElfHash
      \param[in] name name
	  \param[in] len length
	  \return hash
  */
  inline unsigned long basicHash(const unsigned char * name,unsigned long len);
  /*! Hashes some value
      \param[in] key  key value, that will be hashed
	  \param[in] size table size
	  \return index in the table
  */
  template<typename T>
  inline unsigned long getHash(const T & name,unsigned long size);
  /*! Class of hash functions
  */
  template<typename T> 
  class    HashFunction
  {
    public:
               static  unsigned long call(const T & name,unsigned long size);
  };
   /*! Class of string hash function
  */
   template<>
   class HashFunction<hst::string>
   {
     public:
               static unsigned long call(const hst::string & name,unsigned long size);
   };
  /*! \class hash
      Class of a simple hash-based dictionary
      Objects must be comparable to be placed in it.
	  Note: it's optimized only on search, so adding and removing from it
	  will be slooow.
  */
  template<typename Key, typename T>
  class hash
  {
   /*! Defines a chain of collision
   */
   typedef hst::deque< hst::pair<Key,T> > slot;
   friend class const_iterator;
   friend class iterator;
   private:
	       /*! Defines a hash inner data */
	       hst::vector<slot> m_data;
		   /*! Table size */
		   unsigned long     m_table_size;
		   /*! Count of elements */
		   unsigned long     m_count;
		   /*! Inits a table
		   */
		   void init();
		   /*! Extends a table
		   */
		   void extend();
		   /*! Checks, whether can be extended
		   */
		   bool  checkExtend();
   public:
	       /*! Constant iterator
		   */
	       class const_iterator
		   {
		    friend class hash<Key,T>;
		    private:
				    hash<Key,T>       *  m_parent;                //!< Parent
				    unsigned long        m_slotposition;          //!< Position of slot in vector
					typename slot::const_iterator m_it;           //!< Iterator
					const_iterator(hash<Key,T> * parent, unsigned long slot,  const typename hash<Key,T>::slot::const_iterator & it);
					/*! Goes to next element
					*/
					void goNext();
					/*! Goes to previous
					*/
					void goPrevious();
		    public:
				  /*! Broken iterator
				  */
				  const_iterator();
				  /*! Copy constructor
				      \param[in] o other iterator
				  */
				  const_iterator(const const_iterator & o);
				  /*! Assigns an iterator
				      \param[in] o other iterator
					  \return selfreference
				  */
				  const_iterator& operator=(const const_iterator & o);
				  /*! Destructor
				  */
				  ~const_iterator();

				  const Key& key();   //!< Returns a key
				  const T &  value(); //!< Returns a value

				  const_iterator & operator++();   //!< To next value (prefix).
                  const_iterator operator++(int);  //!< To next value (postfix).
                  const_iterator & operator--();   //!< To previous value (prefix).
                  const_iterator operator--(int);  //!< To previous value (postfix).
				 
				  bool operator==(const const_iterator & o) const;
				  bool operator!=(const const_iterator & o) const;

		   };
		   class iterator
		   {
		    friend class hash<Key,T>;
		    private:
				    hash<Key,T>       *  m_parent;                //!< Parent
				    unsigned long        m_slotposition;          //!< Position of slot in vector
					typename slot::iterator m_it;           //!< Iterator
					/*! Constructs an iterator
					    \param[in] parent parent
						\param[in] slot   position
						\param[in] it     simple iterator
					*/
					iterator(hash<Key,T> * parent, unsigned long slot,  const typename hash<Key,T>::slot::iterator & it);
					/*! Goes to next element
					*/
					void goNext();
					/*! Goes to previous
					*/
					void goPrevious();
		    public:
				  /*! Broken iterator
				  */
				  iterator();
				  /*! Copy constructor
				      \param[in] o other iterator
				  */
				  iterator(const iterator & o);
				  /*! Assigns an iterator
				      \param[in] o other iterator
					  \return selfreference
				  */
				  iterator& operator=(const iterator & o);
				  /*! Destructor
				  */
				  ~iterator();

				  const Key& key(); //!< Returns a key
				  T &  value();     //!< Returns a value

				  iterator & operator++();   //!< To next value (prefix).
                  iterator operator++(int);  //!< To next value (postfix).
                  iterator & operator--();   //!< To previous value (prefix).
                  iterator operator--(int);  //!< To previous value (postfix).
				 
				  bool operator==(const iterator & o) const;
				  bool operator!=(const iterator & o) const;
		   };
		   
		   /*! Returns a begin
		   */
		   iterator begin();
           /*! Returns an end iterator
		   */
		   iterator end();
		   
		   /*! Returns a begin iterator
		   */
		   const_iterator const_begin() const;
           /*! Returns an end iterator
		   */
		   const_iterator const_end() const;

	       /*! Inits a small hash with size of 7 elements
		   */
	       hash();
		   /*! Copies a hash
		       \param[in] o other hash
		   */
		   hash(const hash & o);
		   /*! Assigns one hash, to other
		       \param[in] o other hash
			   \return self-reference
		   */
		   hash & operator=(const hash & o);
		   /*! Destructor
		   */
		   ~hash();
		   /*! Clears a hash
		   */
		   void clear();
		   /*! Seeks a key-value pair in hash. If not found, the default value is
		       returned;
			   \param[in] k key
			   \return value
		   */
		   const T & operator[](const Key & k) const;
		   /*! Seeks a key-value pair in hash. If not found, the default value is
		       returned;
			   \param[in] k key
			   \return value
		   */
		   T & operator[](const Key & k);
		   /*! Detects, whether exists some key
               \param[in] k key
		   */
		   bool contains(const Key & k) const;
		   /*! Appends a value to a table
		       \param[in] k key;
			   \param[in] v value
		   */
           void insert(const Key & k, const T & v);
		   /*! Removes a value from a table
		       \param[in] k key
		   */
		   void remove(const Key & k);
		   /*! Returns amount of elements in table
		   */
		   unsigned long count() const;
		   /*! Returns current size of table
		   */
		   unsigned long tableSize() const;
  };

}

#define hhash hst::hash


#include "hhash_src.hpp"

#endif

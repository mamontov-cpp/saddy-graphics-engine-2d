/*! \file    hmap.hpp
    \author  HiddenSeeker

	\brief   Definition of multi-map template class.

	This file contains a definition of multi-map class.
*/
#pragma once
#include "habstract.hpp"
#include "hlvector.hpp"
#include "hdeque.hpp"

namespace hst
{
/*! \class  map
    \brief  Multi-map template class

	Defines a template class of multi-map. To create instance classes must
	support following operators:
	Key: Key(), Key(const Key &),operator=(const Key&),
	     operator<(const Key&),operator>(const Key&);
    T:   T(), T(const T&),operator=(const T&).
*/
template<class Key,class T> class map
{
 private:
        hSortOrder ord;                                      //!< Sorting order of map
        hlvector<Key>          _keys;                        //!< Key array
        hlvector<hdeque<T> >   _values;                      //!< Value 2-d vector
        /*! Compares two keys in a sorting order. Returns true, if comparing
            attribute execs
            \param[in] o1 first key
            \param[in] o2 second key
        */
        bool cmp(const Key & o1, const Key & o2) const;
        /*! Searches a key in map by binary search algorythm
            \param[in] b lower bound of search range
            \param[in] h upper bound of search range
            \param[in] v searching key
            \return    index of key in array. Returns -1 if not found
        */
        long bkeysearch(long b,long h, const Key & v) const;
 public:
        /*! Default constructor. Takes ascending
            sort order as a default.
        */
	    map();
        /*! Parametric constructor. Takes following sorting priority
            \param[in] _ord sorting order
        */
        map(hSortOrder _ord);
        /*! Constructs map, from following amount of values
            \param[in] keys    key array
            \param[in] values  values aray
            \param[in] ord     sorting order
        */
		map(const hlvector<Key> & keys,const hlvector<T> & values,hSortOrder ord=hst::Ascending);
	    /*! Copy constructor. Copys contents of map to other map
	        \param[in] o  other map
	    */
	    map(const map & o);
        /*! Assignment operator overload
            \param[in] o  other map
            \return self-reference
        */
        map & operator=(const map & o);
        /*! Default-destructor
        */
        ~map();

        /*! Fully compares two maps. They are equal, if all their
            parameters are equal.
            \param[in] o other map
            \return true if equal
        */
        bool operator==(const map & o);
        /*! Fully compares two maps. They are equal, if all their
            parameters are equal.
            \param[in] o other map
            \return true if not equal
        */
        bool operator!=(const map & o);


        /*! Adds new element to map. It key does not exists, it will create new
            pair key-element, otherwise adds to existing value vector
            \param[in] v key
            \param[in] t value
            \return self-reference
        */
        map & add(const Key & v, const T & t);
	    /*! Searches a key in a map. Returns it's index in values
	        \param[in] v searched key
	        \return index of key, otherwise -1.
	    */
	    long searchKey(const Key & v) const;
        /*! Detects emptyness
            \return true, if empty, otherwise false
        */
	    bool empty() const;
	    /*! Returns amount  of elements
	        \return amount of elements in map
	    */
	    long          count()  const;
        /*! Returns key reference
            \return key array reference
        */
	    const hlvector<Key> & keys()   const;
        /*! Returns key reference
            \return values 2-d array reference
        */
        const hlvector<hdeque<T> > & values() const;

        /*! Clears a map
        */
        void clear();
        /*! Reverses a map to other sorting order.
        */
        void reverse();

        /*! Detects, whether map contains an element with key.
            \param[in] v searched key
            \return true if contains
        */
        bool contains(const Key  & v) const;

        /*! Removes elements with specified key from a map.
            \param[in] v key
            \return self-reference
        */
        map & remove(const Key & v);


        /*! Returns a reference to first element of existing with specified key.
            If key does not exists, map adds a default object to self and returns
            reference to it.
            \param[in] k key
            \return refserence to element
        */
	    T & operator[](const Key & k);
	    /*! Returns a constant reference to
            first element of existing with specified key.
	        If key does not exists map returns some reference to unknown place.
	        \param[in] k key
	        \return reference to element
	    */
        const T & operator[](const Key & k) const;

        /*! Returns a reference to array of elements with specified key.
            If no elemets is found, adds new array with a default value
            \param[in] k key
            \return reference to element
        */
        hdeque<T> & elements(const Key & k);
        /*! Returns a reference to array of elements with specified key.
            If no elemets is found, returns a reference to unknown place
            where new array will be constructed.
            \param[in] k key
            \return reference to element
        */
        const hdeque<T> & elements(const Key & k) const;



	    /*! Debug print of elements of map
            \param[in] kfmt format string for keys
            \param[in] fmt  format string for values
        */
        void    dbg_ass(const char * kfmt,const char * fmt) const;
        /*! Debug print of elements of map, using callbacks
            \param[in] kfun key callback function
            \param[in] fun  value callback function
        */
        void    dbg_ass(void (*kfun)(const T & o),void (*fun)(const T & o)) const;


        /*! \class  const_iterator
            \brief  Constant iterator for class

	        Defines an iterator class, used on iterations.
        */
	    class const_iterator
        {
          friend class map;
          private:
           const map * parent;      //!< Parent map for iterator
           long pos;                 //!< Position of hdeque where iterator works
           long depth;               //!< Depth of iterator in this hdeque (Current element)
           /*! Constuctor, used map to create iterator
               \param[in] parent  address of map
               \param[in] mypos   position of iterator in map (key)
               \param[in] mydepth position of element, where iterator points
           */
           const_iterator(const map * parent, long mypos, long mydepth);
           /*! Brokes an iterator, by setting it parent to Null
           */
           void broke();
          public:
           /*! Default constructor. It mustn't be used as an itializator.
               It should be used, if you want to create array of iterators
           */
           const_iterator();
           /*! Copy constructor
               \param[in] o other iterator
           */
           const_iterator(const const_iterator & o);
           /*! Assignment operator overload
               \param[in] o other iterator
               \return self-reference
           */
           const_iterator & operator=(const const_iterator & o);
		   /*! Compares two iterators. They are equal, if they are pointing
		       to same element
			   \param[in] o other iterator
			   \return true if they are equal
		   */
		   bool operator==(const class const_iterator & o) const;
           /*! Compares two iterators. They are equal, if they are pointing
		       to same element
			   \param[in] o other iterator
			   \return true if they are not equal
		   */
		   bool operator!=(const class const_iterator & o) const;

		   /*! Default destructor
           */
           ~const_iterator();

           /*! Returns a key
               \return a key, where iterator points
           */
           const Key & key() const;
           /*! Returns a value
               \return a value, where iterator points
           */
           const T   & value() const;

           const_iterator & operator++();   //!< To next key (prefix). Resets depth to 0
           const_iterator operator++(int);  //!< To next key (postfix). Resets depth to 0
           const_iterator & operator--();   //!< To previous key (prefix). Resets depth to 0
           const_iterator operator--(int);  //!< To previous key (postfix). Resets depth to 0

           const_iterator & up();           //!< Jump up by value (--depth)
           const_iterator & dn();           //!< Jump down by value(++depth)

           const_iterator begin();          //!< Marker, that marks to a first element
           const_iterator end();            //!< End marker, that ends array

           /*! Detects, whether is iterator is works.
               \return true, if works.
           */
           bool works() const;
        };

        /*! Returns an iterator to beginning of map
        */
        const_iterator const_begin()       const;
        /*! Returns an iterator to first element, after map
        */
        const_iterator const_end()         const;
        /*! Returns iterator, pointing to first element at position.
            Can return broken iterator, if position can't be found
            \param[in] i position of array of values
            \return new position
        */
        const_iterator at(long i)          const;
        /*! Returns iterator, pointing to  element at position.
            Can return broken iterator, if position can't be found
            \param[in] i position of array of elements
            \param[in] j position of element in array
            \return new position
        */
        const_iterator at(long i,long j)   const;

        /*! Returns an iterator, that points to first element at array
            Can return broken iterator,if can't be found
            \param[in] k key
            \return position
        */
        const_iterator seek(const Key & k) const;


	    class iterator
        {
          friend class map;
          private:
           map * parent;            //!< Parent map for iterator
           long pos;                 //!< Position of hdeque where iterator works
           long depth;               //!< Depth of iterator in this hdeque (Current element)
           /*! Constuctor, used map to create iterator
               \param[in] parent  address of map
               \param[in] mypos   position of iterator in map (key)
               \param[in] mydepth position of element, where iterator points
           */
           iterator(map * parent, long mypos, long mydepth);
           /*! Brokes an iterator, by setting it parent to Null
           */
           void broke();
          public:
           /*! Default constructor. It mustn't be used as an itializator.
               It should be used, if you want to create array of iterators
           */
           iterator();
           /*! Copy constructor
               \param[in] o other iterator
           */
           iterator(const iterator & o);
           /*! Assignment operator overload
               \param[in] o other iterator
               \return self-reference
           */
           iterator & operator=(const iterator & o);
		   /*! Compares two iterators. They are equal, if they are pointing
		       to same element
			   \param[in] o other iterator
			   \return true if they are equal
		   */
		   bool operator==(const class iterator & o) const;
           /*! Compares two iterators. They are equal, if they are pointing
		       to same element
			   \param[in] o other iterator
			   \return true if they are not equal
		   */
		   bool operator!=(const class iterator & o) const;

		   /*! Default destructor
           */
           ~iterator();

           /*! Returns a key
               \return a key, where iterator points
           */
           const Key & key() const;
           /*! Returns a value
               \return a value, where iterator points
           */
           T   & value() const;

           iterator & operator++();   //!< To next key (prefix). Resets depth to 0
           iterator operator++(int);  //!< To next key (postfix). Resets depth to 0
           iterator & operator--();   //!< To previous key (prefix). Resets depth to 0
           iterator operator--(int);  //!< To previous key (postfix). Resets depth to 0

           iterator & up();           //!< Jump up by value (--depth)
           iterator & dn();           //!< Jump down by value(++depth)

           iterator begin();          //!< Marker, that marks to a first element
           iterator end();            //!< End marker, that ends array

           /*! Detects, whether is iterator is works.
               \return true, if works.
           */
           bool works() const;
           /*! Inserts an element to a specified position. Nothing will be done
               if position is not found and element can't be inserted. After insertion
               iterator will point to inserted element.
               \param[in] obj inserted element
               \return self-reference
            */
            iterator & insert(const T & obj);
            /*! Removes an element in specified position, if not broken
                After removing, will point to next element, to next key (if not empty)
                or be broken
            */
            iterator & erase();
        };


        /*! Returns an iterator to beginning of map
        */
        iterator begin();
        /*! Returns an iterator to first element, after map
        */
        iterator end();
        /*! Returns iterator, pointing to first element at position.
            Can return broken iterator, if position can't be found
            \param[in] i position of array of values
            \return new position
        */
        iterator at(long i);
        /*! Returns iterator, pointing to  element at position.
            Can return broken iterator, if position can't be found
            \param[in] i position of array of elements
            \param[in] j position of element in array
            \return new position
        */
        iterator at(long i,long j);

        /*! Returns an iterator, that points to first element at array
            Can return broken iterator,if can't be found
            \param[in] k key
            \return position
        */
        iterator seek(const Key & k);
};

}

/*! Convenience type definition
*/
#define hmap hst::map


#ifndef HI_MAPCONTAINER_H
       #include "hmap_src.hpp"
#endif
#define HI_MAPCONTAINER_H

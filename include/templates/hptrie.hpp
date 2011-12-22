/*! \file   hptrie.hpp
    \author HiddenSeeker

    \brief contains a ptrie class definition.

    Contains a ptrie class definition
*/
#include "hpair.hpp"
#include "hdeque.hpp"
#pragma once

#ifndef __H_PTRIE_HPP
	#define __H_PTRIE_HPP
namespace hst
{
    /*! \class ptrie
        \brief tree-based associative array
		
        Defines a ptrie class, that provides fast associative array,
        based on tree. You must be careful, because it works by byte
        comparing elements of key.
    */
    template<typename Key,typename T>
    class ptrie
    {
        /*! Typedef for element of list. Third parameter is a pointer to
            element iterator
        */
        typedef hst::triplet<Key,T,void *> hiddenel;
        protected:        //This is saved for successor
                hst::deque< hiddenel > m_list; //!< List for fast iterations
                class node
                {
                  friend class ptrie;
                  public:
                    typename hst::deque< hst::triplet<Key,T,void *> >::iterator m_val; //!< Value iterator
                    node ** m_children;                                   //!< Child pointers
                    /*! Gets a children from node
                        \param[in] byte byte value
                    */
                    typename ptrie::node *&  getChildren(unsigned char byte);
                  public:
                    /*! Default constructor
                    */
                    node();
                    /*! Constructor
                        \param[in] m_t new value
                    */
                    node(const typename hst::deque< hst::triplet<Key,T,void *> >::iterator & m_t);
                    /*! Default destructor
                    */
                    ~node();
                } * m_root;                      //!< Root node of tree
                /*! Jumps to node of specified key, if possible.
                    Otherwise returns NULL;
                    \param[in] key key value
                */
                typename ptrie::node * jump(const Key & key) const;
                /*! Clears all tree
                */
                void makeClear();
        public:
               /*! \class const_iterator
                   \brief iterator of main tree

               */
               class const_iterator
               {
                 friend class ptrie;
                 private:
                   /*! Iterator, that he wraps
                   */
                   typename hst::deque< hst::triplet<Key,T,void *> >::const_iterator m_imp;
                   /*! Constant iterator
                       \param[in] rt constant iterator
                   */
                   const_iterator(  const typename hst::deque< hst::triplet<Key,T,void *> >::const_iterator & rt);
                 public:
                     /*! Default iterator
                     */
                     const_iterator();
                     /*! Copy constructor
                         \param[in] rt iterator
                     */
                     const_iterator(const const_iterator & rt);
                     /*! Assignment operator
                        \param[in] rt other iterator
                        \return self-reference
                     */
                     const_iterator & operator=(const const_iterator & rt);
                     /*! Destructor
                     */
                     ~const_iterator();


                     /*! Compares two iterators
                         \param[in] rt other iterator
                         \return true if equal
                     */
                     bool operator==(const const_iterator & rt) const;
                     /*! Compares two iterators
                         \param[in] rt other iterator
                         \return true if not equal
                     */
                     bool operator!=(const const_iterator & rt) const;

                     const_iterator & operator++();   //!< To next value (prefix).
                     const_iterator operator++(int);  //!< To next value (postfix).
                     const_iterator & operator--();   //!< To previous value (prefix).
                     const_iterator operator--(int);  //!< To previous value (postfix).

                     /*! Determines, whether it works
                         \return true if works
                     */
                     inline bool works() const;
                     /*! Determines, whether it dereferencable
                     */
                     inline bool dereferencable() const;
                     /*! Returns key, if possible
                         \return key
                     */
                     const Key & key() const;
                     /*! Returns value, if possible
                         \return value
                     */
                     const T & value() const;
               };
               /*! \class iterator
                   \brief iterator of main tree

               */
               class iterator
               {
                 friend class ptrie;
                 private:
                   /*! Iterator, that he wraps
                   */
                   typename hst::deque< hst::triplet<Key,T,void *> >::iterator m_imp;
                   /*! Constant iterator
                       \param[in] rt iterator
                   */
                   iterator(  const typename hst::deque< hst::triplet<Key,T,void *> >::iterator & rt);
                 public:
                     /*! Default iterator
                     */
                     iterator();
                     /*! Copy constructor
                         \param[in] rt iterator
                     */
                     iterator(const iterator & rt);
                     /*! Assignment operator
                        \param[in] rt other iterator
                        \return self-reference
                     */
                     iterator & operator=(const iterator & rt);
                     /*! Destructor
                     */
                     ~iterator();


                     /*! Compares two iterators
                         \param[in] rt other iterator
                         \return true if equal
                     */
                     bool operator==(const iterator & rt) const;
                     /*! Compares two iterators
                         \param[in] rt other iterator
                         \return true if not equal
                     */
                     bool operator!=(const iterator & rt) const;

                     iterator & operator++();   //!< To next value (prefix).
                     iterator operator++(int);  //!< To next value (postfix).
                     iterator & operator--();   //!< To previous value (prefix).
                     iterator operator--(int);  //!< To previous value (postfix).

                     void erase();              //!< Erases element, behavour stolen from hst::deque::iterator. See hst::deque::iterator::erase() for detail

                     /*! Determines, whether it works
                         \return true if works
                     */
                     inline bool works() const;
                     /*! Determines, whether it dereferencable
                     */
                     inline bool dereferencable() const;
                     /*! Returns key, if possible
                         \return key
                     */
                     const Key & key() ;
                     /*! Returns value, if possible
                         \return value
                     */
                     T & value();
               };
               /*! Returns constant iterator to beginning
                   \return beginning
               */
               const_iterator const_begin() const;
               /*! Returns constant iterator to end
                   \return beginning
               */
               const_iterator const_end() const;
               /*! Returns  iterator to beginning
                   \return beginning
               */
               iterator begin() ;
               /*! Returns iterator to end
                   \return beginning
               */
               iterator end() ;
               /*! Returns iterator to last element
                   \return last
               */
               iterator last() ;
               /*! Default ptrie constructor
               */
               ptrie();
               /*! Construct ptrie from other ptrie
                   \param[in] o other ptrie
               */
               ptrie(const ptrie & o);
               /*! Assigns ptrie other ptrie
                   \param[in] o oher ptrie
                   \return self-reference
               */
               ptrie & operator=( const ptrie & o);
               /*! Default destructor
               */
               ~ptrie();
               /*! Adds an element to tree. If an element exists, repllaces it
                   with new value.
                   \param[in]  key key
                   \param[in]  val inserted value
               */
               ptrie & add(const Key & key, const T & val);
               /*! Returns a constant pointer to element, if it exists in ptrie.
                   Otherwise returns NULL.
                   \param[in] key key
               */
               T const * operator[](const Key & key) const;
               /*! Returns a pointer to element, if it exists in ptrie.
                   Otherwise returns NULL.
                   \param[in] key key
               */
               T  * operator[](const Key & key) ;
               /*! Removes element from ptrie, if possible
                   \param[in] key key of removing element
               */
               ptrie & remove(const Key & key);

               inline bool empty() const;                               //!< Detects, whether list is empty.
               inline long count() const;                               //!< Return amount of elements in list.
               /*! Clears all
               */
               void clear();
    };
}

#ifndef HI_PTRIE
       #define HI_PTRIE
       #include "hptrie_src.hpp"
#endif

#endif

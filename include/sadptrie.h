/*! \file   sadptrie.h
    

    \brief contains a Ptrie class definition.

    Contains a Ptrie class definition
*/
#pragma once
#include "sadpair.h"
#include "sadlinkedlist.h"

namespace sad
{
    /*! \class Ptrie
        \brief tree-based associative array
        
        Defines a Ptrie class, that provides fast associative array,
        based on trie. You must be careful, because it works by byte
        comparing elements of key.
    */
    template<typename Key,typename T>
    class Ptrie
    {
        /*! Typedef for element of list. Third parameter is a pointer to
            element iterator
        */
        typedef sad::Triplet<Key,T,void *> hiddenel;
        protected:        //This is saved for successor
                sad::LinkedList< hiddenel > m_list; //!< List for fast iterations
                class node
                {
                  friend class Ptrie;
                  public:
                    typename sad::LinkedList< sad::Triplet<Key,T,void *> >::iterator m_val; //!< Value iterator
                    node ** m_children;                                   //!< Child pointers
                    /*! Gets a children from node
                        \param[in] byte byte value
                    */
                    typename Ptrie::node *&  getChildren(unsigned char byte);
                  public:
                    /*! Default constructor
                    */
                    node();
                    /*! Constructor
                        \param[in] m_t new value
                    */
                    node(const typename sad::LinkedList< sad::Triplet<Key,T,void *> >::iterator & m_t);
                    /*! Default destructor
                    */
                    ~node();
                } * m_root;                      //!< Root node of tree
                /*! Jumps to node of specified key, if possible.
                    Otherwise returns nullptr;
                    \param[in] key key value
                */
                typename Ptrie::node * jump(const Key & key) const;
                /*! Clears all tree
                */
                void makeClear();
        public:
               /*! \class const_iterator
                   \brief iterator of main tree

               */
               class const_iterator
               {
                 friend class Ptrie;
                 private:
                   /*! Iterator, that he wraps
                   */
                   typename sad::LinkedList< sad::Triplet<Key,T,void *> >::const_iterator m_imp;
                   /*! Constant iterator
                       \param[in] rt constant iterator
                   */
                   const_iterator(  const typename sad::LinkedList< sad::Triplet<Key,T,void *> >::const_iterator & rt);
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
                 friend class Ptrie;
                 private:
                   /*! Iterator, that he wraps
                   */
                   typename sad::LinkedList< sad::Triplet<Key,T,void *> >::iterator m_imp;
                   /*! Constant iterator
                       \param[in] rt iterator
                   */
                   iterator(  const typename sad::LinkedList< sad::Triplet<Key,T,void *> >::iterator & rt);
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

                     void erase();              //!< Erases element, behavour stolen from sad::LinkedList::iterator. See sad::LinkedList::iterator::erase() for detail

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
               /*! Default Ptrie constructor
               */
               Ptrie();
               /*! Construct Ptrie from other Ptrie
                   \param[in] o other Ptrie
               */
               Ptrie(const Ptrie & o);
               /*! Assigns Ptrie other Ptrie
                   \param[in] o oher Ptrie
                   \return self-reference
               */
               Ptrie & operator=( const Ptrie & o);
               /*! Default destructor
               */
               ~Ptrie();
               /*! Adds an element to tree. If an element exists, repllaces it
                   with new value.
                   \param[in]  key key
                   \param[in]  val inserted value
               */
               Ptrie & add(const Key & key, const T & val);
               /*! Returns a constant pointer to element, if it exists in Ptrie.
                   Otherwise returns nullptr.
                   \param[in] key key
               */
               T const * operator[](const Key & key) const;
               /*! Returns a pointer to element, if it exists in Ptrie.
                   Otherwise returns nullptr.
                   \param[in] key key
               */
               T  * operator[](const Key & key) ;
               /*! Removes element from Ptrie, if possible
                   \param[in] key key of removing element
               */
               Ptrie & remove(const Key & key);

               inline bool empty() const;                               //!< Detects, whether list is empty.
               inline long count() const;                               //!< Return amount of elements in list.
               /*! Clears all
               */
               void clear();
    };
}

#include "sadptrie_src.h"

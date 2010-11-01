/*! \file    hdeque.hpp
    \author  HiddenSeeker

	\brief   Definition of doubly linked list.

	This file contains a definition of doubly linked list.
*/
#pragma once

namespace hst
{
/*! \class  deque
    \brief  Doubly linked list

	Defines a template class of doubly linked list, with queuue functions.
*/
template<class T> class deque
{
 public: class iterator;
 private:
		 /*!
		     \brief Class of node of list

			 Defines a list node class.
		 */
		 class node
		 {
		  public:
			     node * previous;                             //!< Pointer to previous node
			     T    * me;                                   //!< Pointer to element
			     node * next;                                 //!< Pointer to next node
				 /*! Constructor to create some node
				     \param[in] previous pointer to previous node
					 \param[in] next     pointer to next node
					 \param[in] me       pointer to object
				 */
				 node(node * previous, node * next, T * me);
				 /*! Simple destructor, which destructs object
				 */
				 ~node();
		 };

		 node * firstnode;                                   //!< Pointer to head of list
		 node * lastnode;        							 //!< Pointer to last object of list

		 node * currentnode;                                 //!< Pointer to selected node
         long   currentindex;    							 //!< Current index. An index, which indicates a current node position

		 long   sz;              							 //!< Size of list.
         /*! Procedure that cleans all list
		 */
		 void makeClear();
		 /*! Jump to previous nodes by amount of jumps
             \param[in] from  beginning node
             \param[in] jumps amount of jumps, to be done
             \return    pointer to addressed node
         */
         void * jumpPrevious(node * from,long jumps) const;
         /*! Jump to next nodes by amount of jumps
             \param[in] from  beginning node
             \param[in] jumps amount of jumps, to be done
             \return    pointer to addressed node
         */
         void * jumpNext(node * from,long jumps) const;
         /*! Gets node by index
		     \param[in] i index
			 \return pointer to node
         */
		 node * getNode(long i) const;

         /*! Inserts value in iterator position. Element, that was here before shifts as next
             \param[in,out] it  iterator, that will point to inserted element
             \param[in]     val value
         */
         void insert(typename deque::iterator & it, const T & val);
		 /*! Removes value at iterator position. Iterator will point
		     to previous element, or next, if element is last, or be broken,
		     if parent is empty.
		     \param[in,out] it iterator.
         */
		 void remove(typename deque::iterator & it);

 public:
          /*!
		     \brief const_iterator

			 Defines a const iterator
		  */
          class const_iterator
          {
            friend class deque;
            private:
                    deque const * m_parent;                //!< Parent reference
                    typename deque::node const * m_next;   //!< Next node
                    typename deque::node const * m_me;     //!< Reference to node
                    typename deque::node const * m_prev;   //!< Previous node, used by last iterator
            /*! Private constructor
                \param[in] parent parent deque
                \param[in] next   next node
                \param[in] me     self   node
                \param[in] prev   previous node
            */
            const_iterator(deque const * parent,typename deque::node const * next,typename deque::node const * me,typename deque::node const * prev);
            /*! Breaks an iterator
            */
            inline void broke();
            public:
            /*! Default constructor
            */
            const_iterator();
            /*! Default destructor
            */
            ~const_iterator();
            /*! Copy constructor
                \param[in] o other iterator
            */
            const_iterator(const const_iterator & o);
            /*! Assignment overload
                \param[in] o other iterator
                \return self-reference
            */
            const_iterator & operator=(const const_iterator & o);
            /*! Defines, whether iterators are equal
                \param[in] o other iterator
                \return true if equal
            */
            bool operator==(const const_iterator &  o) const;
            /*! Defines, whether iterators are equal
                \param[in] o other iterator
                \return true if equal
            */
            bool operator!=(const const_iterator &  o) const;

            /*! Returns an value pointer
                \return dereference
            */
            const T & operator*() const;

           const_iterator & operator++();   //!< To next value (prefix).
           const_iterator operator++(int);  //!< To next value (postfix).
           const_iterator & operator--();   //!< To previous value (prefix).
           const_iterator operator--(int);  //!< To previous value (postfix).

            /*! Determines, whether iterator has next node
                \return true, if so.
            */
            inline bool hasNext() const;
            /*! Determines, whether iterator has previous node
                \return true, if so.
            */
            inline bool hasPrevious() const;
            /*! Determines, whether iterator is dereferencable
                \return true, if so.
            */
            inline bool dereferencable() const;
            /*! Determines, whether iterator is broken
                \return true, if works
            */
            inline  bool works() const ;
          };
          /*!
		     \brief iterator

			 Defines a iterator
		  */
          class iterator
          {
            friend class deque;
            private:
                    deque * m_parent;                      //!< Parent reference
                    typename deque::node  * m_next;   //!< Next node
                    typename deque::node  * m_me;     //!< Reference to node
                    typename deque::node  * m_prev;   //!< Previous node, used by last iterator
            /*! Private constructor
                \param[in] parent parent deque
                \param[in] next   next node
                \param[in] me     self   node
                \param[in] prev   previous node
            */
            iterator(deque  * parent,typename deque::node * next,typename deque::node * me,typename deque::node * prev);
            /*! Breaks an iterator
            */
            inline void broke();
            public:
            /*! Default constructor
            */
            iterator();
            /*! Default destructor
            */
            ~iterator();
            /*! Copy constructor
                \param[in] o other iterator
            */
            iterator(const iterator & o);
            /*! Assignment overload
                \param[in] o other iterator
                \return self-reference
            */
            iterator & operator=(const iterator & o);
            /*! Defines, whether iterators are equal
                \param[in] o other iterator
                \return true if equal
            */
            bool operator==(const iterator &  o) const;
            /*! Defines, whether iterators are equal
                \param[in] o other iterator
                \return true if equal
            */
            bool operator!=(const iterator &  o) const;

            /*! Returns an value pointer
                \return dereference
            */
            T & operator*();

           iterator & operator++();   //!< To next value (prefix).
           iterator operator++(int);  //!< To next value (postfix).
           iterator & operator--();   //!< To previous value (prefix).
           iterator operator--(int);  //!< To previous value (postfix).

            /*! Inserts value in specified position
                \param[in] val value
            */
            iterator & insert(const T & val);
            /*!  Removes value at iterator position. Iterator will point
		         to previous element, or next, if element is last, or be broken,
		         if parent deque is empty.
            */
            iterator & erase();
            /*! Determines, whether iterator has next node
                \return true, if so.
            */
            inline bool hasNext() const;
            /*! Determines, whether iterator has previous node
                \return true, if so.
            */
            inline bool hasPrevious() const;
            /*! Determines, whether iterator is dereferencable
                \return true, if so.
            */
            inline bool dereferencable() const;
            /*! Determines, whether iterator is broken
                \return true, if works
            */
            inline  bool works() const ;
          };
          /*! Returns a constant iterator to begin
          */
          const_iterator const_begin() const;
          /*! Returns a constant iterator to end
          */
          const_iterator const_end() const;
          /*! Returns a constant iterator to begin
          */
          iterator begin();
          /*! Returns a constant iterator to end
          */
          iterator end();


          deque();                                          //!< Default constructor.
          /*! Copy constructor.
		      \param[in] h other deque
		  */
		  deque(const deque & h);
          /*! Assignment operator overload.
              \param[in] h other deque
		  */
          deque & operator=(const deque & h);
          ~deque();                                         //!< Destructor, which deletes all list nodes.
          //Getters---------------
		   bool empty() const;                               //!< Detects, whether list is empty.
           long count() const;                               //!< Return amount of elements in list.
           T*   data()  const;                               //!< Return a pointer to first node.

		   /*! Debug print
		       \param[in] cp - format string in printf() format
		   */
		   void dbg_ass(const char * cp) const;
           /*! Returns reference to specified element.
		       Returns a default element reference, if element cannot be found
			   \param[in] i index
			   \return reference to element
		   */
		   T&   operator[](long i);
           /*! Returns constant reference to specified element.
		       Returns a default element reference, if element cannot be found
			   \param[in] i index
			   \return constance reference to element
		   */
		   const T &   operator[](long i) const;
           //----------------------
		   /*! Adds an object to list. Equal to deque::add
		       \param[in] obj deque to be added
			   \return self-reference
		   */
           deque & operator<<(const T & obj);
		   /*! Adds a deque to deque. Equal to deque::add
		       \param[in] obj deque to be added
			   \return self-reference
		   */
		   deque & operator<<(const deque & obj);
           /*! Copys a last object in list to reference, with removing it from list.
		       \param[in] obj reference, where object will be stored
			   \return self-reference
		   */
		   deque & operator>>(T & obj);
           /*! Adds a object to list
		       \param[in] obj object to be added
			   \return self-reference
		   */
		   deque & add(const T & obj);
		   /*! Adds a list to list.
		       \param[in] o list to be added
			   \return self-reference
		   */
		   deque & add(const deque & o);
		   /*! Adds an object to front of list. Equal to insert(T,0).
		       \param[in] obj inserted object
			   \return self-reference
		   */
		   deque & addFront(const T & obj);
           /*! Inserts an object to list in specified position.
			   \param[in] obj  Object to add to
			   \param[in] i    Position to add
			   \return self-reference
		   */
		   deque & insert(const T &obj,long i);
           /*! Removes an element in specified position.
		       \param[in] i index of object, which is being removed
			   \return self-reference
           */
           deque & removeAt(long i);
           /*! Removes all occurences of element obj.
		      \param[in] obj object, which occurences are being removed
			  \return self-reference
		   */
		   deque & remove(const T & obj);
           /*! Removes first occurence of element obj.
		      \param[in] obj object, which occurences are being removed
			  \return self-reference
		   */
		   deque & removeFirst(const T & obj);
           /*! Removes last occurence of element obj.
		      \param[in] obj object, which occurences are being removed
			  \return self-reference
		   */
		   deque & removeLast(const T & obj);
           /*! Removes an elements in range from imin to imax
		       \param[in] imin first bound of range
			   \param[in] imax second bound of range
			   \return self-reference
		   */
		   deque & removeRange(long imin,long imax);

		   //Queue procedures
		   /*! This function is added to provide deque function.
		       Pushs an element to front.
			   \param[in] obj object
			   \return self-reference
		   */
		   deque & push_front(const T & obj);
		   /*! This function is added to provide deque function.
		       Pops an element from back.
			   \param[in] obj reference, where object will be stored
			   \return self-reference
		   */
		   deque & pop_front(T & obj);
		   /*! This function is added to provide deque function.
		       Pushs an element to back.
			   \param[in] obj object
			   \return self-reference
		   */
		   deque & push_back(const T & obj);
		   /*! This function is added to provide deque function.
		       Pops an element from front.
			   \param[in] obj reference, where object will be stored
			   \return self-reference
		   */
		   deque & pop_back(T & obj);

		   /*! Cleanups list, removing all elements
           */
		   void		clear();
	       /*! Maps function to every element in list
		       \param[in] fptr pointer to mapping function
		   */
		   void     yield(void (*fptr)(T & obj));
};

}
/*! Convenience typedef
*/
#define hdeque hst::deque

#ifndef HI_DEQUE_H
       #include "hdeque_src.hpp"
#endif
#define HI_DEQUE_H

/*! \file    sadlinkedlist.h
    \brief   Definition of doubly-linked list.

    This file contains a definition of doubly-linked list.
*/
#pragma once

namespace sad
{
/*! \class  LinkedList
    \brief  doubly-linked list

    Defines a template class of doubly linked list, with queue functions.
*/
template<
    typename T
> 
class LinkedList
{
private:
    class node;
public: 
    class iterator;
    /*! Defines a const iterator
     */
    class const_iterator
    {
        friend class LinkedList;
        /*! Private constructor
            \param[in] parent parent linked list
            \param[in] next   next node
            \param[in] me     self   node
            \param[in] prev   previous node
         */
        const_iterator(
            sad::LinkedList<T> const * parent,
            typename sad::LinkedList<T>::node const * next,
            typename sad::LinkedList<T>::node const * me,
            typename sad::LinkedList<T>::node const * prev
        );
        /*! Breaks an iterator, turning it to invalid operator
         */
        inline void make_invalid();
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
        const_iterator(const const_iterator& o);
        /*! Assignment overload
            \param[in] o other iterator
            \return self-reference
         */
        const_iterator& operator=(const const_iterator& o);
        /*! Defines, whether iterators are equal
            \param[in] o other iterator
            \return true if equal
        */
        bool operator==(const const_iterator&  o) const;
        /*! Defines, whether iterators are equal
            \param[in] o other iterator
            \return true if equal
        */
        bool operator!=(const const_iterator&  o) const;
        /*! Returns a referenced value
            \return a referenced value
        */
        const T& operator*() const;

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
    private:
        sad::LinkedList<T> const * m_parent;                //!< Parent reference
        typename sad::LinkedList<T>::node const * m_next;   //!< Next node
        typename sad::LinkedList<T>::node const * m_me;     //!< Reference to node
        typename sad::LinkedList<T>::node const * m_prev;   //!< Previous node, used by last iterator
    };
    /*! Defines a iterator
     */
    class iterator
    {
        friend class LinkedList;
    private:
        /*! Private constructor
            \param[in] parent parent linked list
            \param[in] next   next node
            \param[in] me     self   node
            \param[in] prev   previous node
         */
        iterator(
            sad::LinkedList<T>  * parent,
            typename sad::LinkedList<T>::node * next,
            typename sad::LinkedList<T>::node * me,
            typename sad::LinkedList<T>::node * prev
        );
        /*! Breaks an iterator, turning it to invalid iterator
         */
        inline void make_invalid();
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
             if parent LinkedList is empty.
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
    private:
        sad::LinkedList<T> * m_parent;                      //!< Parent reference
        typename sad::LinkedList<T>::node  * m_next;   //!< Next node
        typename sad::LinkedList<T>::node  * m_me;     //!< Reference to node
        typename sad::LinkedList<T>::node  * m_prev;   //!< Previous node, used by last iterator
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
    /*! Default constructor
     */
    LinkedList();
    /*! Copy constructor.
        \param[in] h other LinkedList
    */
    LinkedList(const sad::LinkedList<T> & h);
    /*! Assignment operator overload.
        \param[in] h other LinkedList
    */
    sad::LinkedList<T> & operator=(const sad::LinkedList<T> & h);
    ~LinkedList();                                    //!< Destructor, which deletes all nodes.
    bool empty() const;                               //!< Detects, whether list is empty.
    long count() const;                               //!< Return amount of elements in list.
    T*   data()  const;                               //!< Return a pointer to first node.
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
    const T&   operator[](long i) const;
    /*! Adds an object to list. Equal to LinkedList::add
       \param[in] obj LinkedList to be added
       \return self-reference
    */
    sad::LinkedList<T>& operator<<(const T& obj);
    /*! Adds a LinkedList to LinkedList. Equal to LinkedList::add
       \param[in] obj LinkedList to be added
       \return self-reference
    */
    sad::LinkedList<T>& operator<<(const sad::LinkedList<T>& obj);
    /*! Copies a last object in list to reference, with removing it from list.
       \param[in] obj reference, where object will be stored
       \return self-reference
    */
    sad::LinkedList<T>& operator>>(T& obj);
    /*! Adds a object to list
       \param[in] obj object to be added
       \return self-reference
    */
    sad::LinkedList<T>& add(const T& obj);
    /*! Adds a list to list.
       \param[in] o list to be added
       \return self-reference
    */
    sad::LinkedList<T>& add(const sad::LinkedList<T>& o);
    /*! Adds an object to front of list. Equal to insert(T,0).
       \param[in] obj inserted object
       \return self-reference
    */
    sad::LinkedList<T>& addFront(const T& obj);
    /*! Inserts an object to list in specified position.
       \param[in] obj  Object to add to
       \param[in] i    Position to add
       \return self-reference
    */
    sad::LinkedList<T>& insert(const T& obj,long i);
    /*! Removes an element in specified position.
       \param[in] i index of object, which is being removed
       \return self-reference
    */
    sad::LinkedList<T>& removeAt(long i);
    /*! Removes all occurrences of element obj.
      \param[in] obj object, which occurrences are being removed
      \return self-reference
    */
    sad::LinkedList<T>& remove(const T& obj);
    /*! Removes first occurrences of element obj.
      \param[in] obj object, which occurrences are being removed
      \return self-reference
    */
    sad::LinkedList<T>& removeFirst(const T& obj);
    /*! Removes last occurrence of element obj.
      \param[in] obj object, which occurrence are being removed
      \return self-reference
    */
    sad::LinkedList<T>& removeLast(const T& obj);
    /*! Removes an elements in range from imin to imax
       \param[in] imin first bound of range
       \param[in] imax second bound of range
       \return self-reference
    */
    sad::LinkedList<T>& removeRange(long imin, long imax);
    /*! This function is added to provide LinkedList function.
       Pushs an element to front.
       \param[in] obj object
       \return self-reference
     */
    sad::LinkedList<T>& push_front(const T& obj);
    /*! This function is added to provide LinkedList function.
       Pops an element from back.
       \param[in] obj reference, where object will be stored
       \return self-reference
    */
    sad::LinkedList<T>& pop_front(T& obj);
    /*! This function is added to provide LinkedList function.
       Pushs an element to back.
       \param[in] obj object
       \return self-reference
    */
    sad::LinkedList<T>& push_back(const T& obj);
    /*! This function is added to provide LinkedList function.
       Pops an element from front.
       \param[in] obj reference, where object will be stored
       \return self-reference
    */
    sad::LinkedList<T>& pop_back(T& obj);
    /*! Cleanups list, removing all elements
     */
    void        clear();
    /*! Checks, whether position of inner pointer for fast random access is correct
        \return true if connect
     */
    bool correct() { return m_currentindex < m_size || (m_size == 0 && m_currentindex == 0); }
private:
    /*! Defines a node of a list.
     */
    class node
    {
    public:
        node* m_previous;   //!< Pointer to previous node
        T* m_me;            //!< Pointer to element
        node* m_next;       //!< Pointer to next node
        /*! Constructor to create some node
            \param[in] previous pointer to previous node
            \param[in] next     pointer to next node
            \param[in] me       pointer to object
         */
        node(node* previous, node* next, T* me);
        /*! Simple destructor, which destructs object
        */
        ~node();
    };
    /*! Procedure that cleans all list
     */
    void makeClear();
    /*! Jump to previous nodes by amount of jumps
        \param[in] from  beginning node
        \param[in] jumps amount of jumps, to be done
        \return    pointer to addressed node
     */
    void* jumpPrevious(node* from, long jumps) const;
    /*! Jump to next nodes by amount of jumps
        \param[in] from  beginning node
        \param[in] jumps amount of jumps, to be done
        \return    pointer to addressed node
     */
    void* jumpNext(node* from, long jumps) const;
    /*! Gets node by index
        \param[in] i index
        \return pointer to node
     */
    node* getNode(long i) const;
    /*! Inserts value in iterator position. Element, that was here before shifts as next
        \param[in,out] it  iterator, that will point to inserted element
        \param[in]     val value
     */
    void insert(typename LinkedList::iterator& it, const T& val);
    /*! Removes value at iterator position. Iterator will point
        to previous element, or next, if element is last, or be broken,
        if parent is empty.
        \param[in,out] it iterator.
    */
    void remove(typename LinkedList::iterator& it);

    node* m_firstnode;      //!< Pointer to head of list
    node* m_lastnode;       //!< Pointer to last object of list

    node* m_currentnode;    //!< Pointer to selected node
    long   m_currentindex;  //!< Current index. An index, which indicates a current node position

    long   m_size;          //!< Size of list.
};

}

#include "sadlinkedlist_src.h"

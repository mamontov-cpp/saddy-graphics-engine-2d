/*! \file    sadlist.h
    \author  HiddenSeeker

	\brief   Definition of List.

	This file contains a definition of List.
*/
#pragma once
	
namespace sad
{
/*! \class  List
    \brief  List class

	Defines a template class of List. List tries to keep pointers to his
	elements as constant
*/
template<class T> class List
{
 public:
          List();                                          //!< Default constructor.
          /*! Copy constructor.
		      \param[in] h other List
		  */
		  List(const List & h);
          /*! Assignment operator overload.
              \param[in] h other List
		  */
          List & operator=(const List & h);
		  /*! Two Lists are not equal, if their lengths aren't equal, or their
		      sizes aren't equal
			  \param[in] h other List
			  \return true if they aren't equal
		  */
		  bool operator!=(const List & h);
          /*! Two Lists are not equal, if their lengths aren't equal, or their
		      sizes aren't equal
			  \param[in] h other List
			  \return true if they are equal
		  */
		  bool operator==(const List & h);
		  ~List();                                         //!< Destructor, which deletes all List nodes.
          //Getters---------------
		   bool empty() const;                               //!< Detects, whether List is empty.
           long count() const;                               //!< Return amount of elements in List.
           const T**   data()  const;                        //!< Return a pointer to first node.
		   /*! Clears List
		   */
		   void clear();
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
		   /*! Adds an object to List. Equal to List::add
		       \param[in] obj List to be added
			   \return self-reference
		   */
           List & operator<<(const T & obj);
		   /*! Adds a List to List. Equal to List::add
		       \param[in] obj List to be added
			   \return self-reference
		   */
		   List & operator<<(const List & obj);
           /*! Copys a last object in List to reference, with removing it from List.
		       \param[in] obj reference, where object will be stored
			   \return self-reference
		   */
		   List & operator>>(T & obj);
           /*! Adds a object to List
		       \param[in] obj object to be added
			   \return self-reference
		   */
		   List & add(const T & obj);
		   /*! Adds a List to List.
		       \param[in] o List to be added
			   \return self-reference
		   */
		   List & add(const List & o);
		   /*! Adds an object to front of List. Equal to insert(T,0).
		       \param[in] obj inserted object
			   \return self-reference
		   */
		   List & addFront(const T & obj);
           /*! Inserts an object to List in specified position.
			   \param[in] obj  Object to add to
			   \param[in] pos  Position to add
			   \return self-reference
		   */
		   List & insert(const T &obj,long pos);
           /*! Removes an element in specified position.
		       \param[in] i index of object, which is being removed
			   \return self-reference
           */
           List & removeAt(long i);
           /*! Removes all occurences of element obj.
		      \param[in] obj object, which occurences are being removed
			  \return self-reference
		   */
		   List & remove(const T & obj);
           /*! Removes first occurence of element obj.
		      \param[in] obj object, which occurences are being removed
			  \return self-reference
		   */
		   List & removeFirst(const T & obj);
           /*! Removes last occurence of element obj.
		      \param[in] obj object, which occurences are being removed
			  \return self-reference
		   */
		   List & removeLast(const T & obj);
           /*! Removes an elements in range from imin to imax
		       \param[in] imin first bound of range
			   \param[in] imax second bound of range
			   \return self-reference
		   */
		   List & removeRange(long imin,long imax);
 private:
	     T**  _p;      //!< Array of links to elements
		 long sz;      //!< Size of array
		 /*! Procedure that cleans all List
		 */
		 void makeClear();
		 /*! Copys List contents to a self
		     \param[in] h copied List
		 */
		 void copy(const List & h);
};

}

#include "sadList_src.h"

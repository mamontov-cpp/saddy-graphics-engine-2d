/*! \file    hlist.hpp
    \author  HiddenSeeker

	\brief   Definition of list.

	This file contains a definition of list.
*/
#pragma once

#ifndef __H_LINEARLIST_HPP
	#define __H_LINEARLIST_HPP
	
namespace hst
{
/*! \class  list
    \brief  List class

	Defines a template class of list. List tries to keep links to his
	element constant
*/
template<class T> class list
{
 private:
	     T**  _p;      //!< Array of links to elements
		 long sz;      //!< Size of array
		 /*! Procedure that cleans all list
		 */
		 void makeClear();
		 /*! Copys list contents to a self
		     \param[in] h copied list
		 */
		 void copy(const list & h);
 public:
          list();                                          //!< Default constructor.
          /*! Copy constructor.
		      \param[in] h other list
		  */
		  list(const list & h);
          /*! Assignment operator overload.
              \param[in] h other list
		  */
          list & operator=(const list & h);
		  /*! Two lists are not equal, if their lengths aren't equal, or their
		      sizes aren't equal
			  \param[in] h other list
			  \return true if they aren't equal
		  */
		  bool operator!=(const list & h);
          /*! Two lists are not equal, if their lengths aren't equal, or their
		      sizes aren't equal
			  \param[in] h other list
			  \return true if they are equal
		  */
		  bool operator==(const list & h);
		  ~list();                                         //!< Destructor, which deletes all list nodes.
          //Getters---------------
		   bool empty() const;                               //!< Detects, whether list is empty.
           long count() const;                               //!< Return amount of elements in list.
           const T**   data()  const;                        //!< Return a pointer to first node.
		   /*! Clears list
		   */
		   void clear();
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
		   /*! Adds an object to list. Equal to list::add
		       \param[in] obj list to be added
			   \return self-reference
		   */
           list & operator<<(const T & obj);
		   /*! Adds a list to list. Equal to list::add
		       \param[in] obj list to be added
			   \return self-reference
		   */
		   list & operator<<(const list & obj);
           /*! Copys a last object in list to reference, with removing it from list.
		       \param[in] obj reference, where object will be stored
			   \return self-reference
		   */
		   list & operator>>(T & obj);
           /*! Adds a object to list
		       \param[in] obj object to be added
			   \return self-reference
		   */
		   list & add(const T & obj);
		   /*! Adds a list to list.
		       \param[in] o list to be added
			   \return self-reference
		   */
		   list & add(const list & o);
		   /*! Adds an object to front of list. Equal to insert(T,0).
		       \param[in] obj inserted object
			   \return self-reference
		   */
		   list & addFront(const T & obj);
           /*! Inserts an object to list in specified position.
			   \param[in] obj  Object to add to
			   \param[in] pos  Position to add
			   \return self-reference
		   */
		   list & insert(const T &obj,long pos);
           /*! Removes an element in specified position.
		       \param[in] i index of object, which is being removed
			   \return self-reference
           */
           list & removeAt(long i);
           /*! Removes all occurences of element obj.
		      \param[in] obj object, which occurences are being removed
			  \return self-reference
		   */
		   list & remove(const T & obj);
           /*! Removes first occurence of element obj.
		      \param[in] obj object, which occurences are being removed
			  \return self-reference
		   */
		   list & removeFirst(const T & obj);
           /*! Removes last occurence of element obj.
		      \param[in] obj object, which occurences are being removed
			  \return self-reference
		   */
		   list & removeLast(const T & obj);
           /*! Removes an elements in range from imin to imax
		       \param[in] imin first bound of range
			   \param[in] imax second bound of range
			   \return self-reference
		   */
		   list & removeRange(long imin,long imax);
           /*! Maps function to every element in list
		       \param[in] fptr pointer to mapping function
		   */
		   void     yield(void (*fptr)(T & obj));
};

}

/*! Convenience typedef
*/
#define hlist hst::list

#ifndef HI_LINEAR_LIST_H
       #include "hlist_src.hpp"
#endif
#define HI_LINEAR_LIST_H

#endif
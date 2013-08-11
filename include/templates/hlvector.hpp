/*! \file    hlvector.hpp
    \author  HiddenSeeker

	\brief   Definition of vector.

	This file contains a definition of vector.
*/
#include <string.h>
#include <vector>
#pragma once

#ifndef __H_LINEAR_VECTOR
	#define __H_LINEAR_VECTOR
namespace hst
{
/*! \class  vector
    \brief  Vector

	Defines a template class of vector.
*/
template<class T> class vector:public std::vector<T>
{
  public:
          vector();                                        //!< Constructor, that creates an empty vector.
          /*! Copy constructor.
		      \param[in] h other vector
		  */
		  vector(const vector & h);
          /*! Assignment operator overload
		      \param[in] Sh other vector
			  \return self-reference
          */
          vector & operator=(const vector & Sh);
		  /*! Compares vectors by length and by elements end displays equiualence
		      \param[in] o other vector
			  \return true if equiualent
		  */
          bool operator==(const vector & o);
		  /*! Compares vectors by length and by elements end displays non-equiualence
		      \param[in] o other vector
			  \return true if not equiualent
		  */
		  bool operator!=(const vector & o);
		  virtual ~vector();                                       //!< Destructor.
          //Геттеры---------------
           inline unsigned long count() const;                               //!< Returns a size of vector
           inline T*   data()  const;                               //!< Returns a pointer to data
           //----------------------
           /*! Adds object to vector. Equal to vector::add
		       \param[in] obj object to be added
			   \return self-reference
		   */
		   vector & operator<<(const T & obj);
			/*! Appends vector to vector. 
		       \param[in] obj vector to be added
			   \return self-reference
		   */
		   vector & operator<<(const hst::vector<T> & obj);
           /*! Copys a last object in vector, with removing it
		       from vector
			   \param[in] obj reference, where removed object will be stored
			   \return self-reference
		   */
		   vector & operator>>(T & obj);
           /*! Adds object to vector
		       \param[in] obj object
			   \return self-reference
		   */
		   vector & add(const T & obj);
           /*! Adds object to a front of vector. Equal to vector::insert(T,0)
		       \param[in] obj object to be added
			   \return self-reference
		   */
		   vector & addFront(const T & obj);
           /*! Inserts an object to vector in specified position.
			   \param obj  Object to add to
			   \param i    Position to add
			   \return self-reference
		   */
		   vector & insert(const T &obj,unsigned long i);
           /*! Removes an element in specified position
		       \param[in] i position of object, which will be removed
			   \return self-reference
		   */
		   vector & removeAt(unsigned long i);
           /*! Removes occurences of element  obj
		       \param[in] obj object to be removed
			   \return self-reference
		   */
		   vector & removeAll(const T & obj);
           /*! Removes first occurence of element  obj
		       \param[in] obj object to be removed
			   \return self-reference
		   */
		   vector & removeFirst(const T & obj);
           /*! Removes last occurence of element  obj
		       \param[in] obj object to be removed
			   \return self-reference
		   */
		   vector & removeLast(const T & obj);
           /*! Removes an elements in range from imin to imax
		       \param[in] imin minimum  range index
			   \param[in] imax maximum  range index
			   \return self-reference
		   */
		   vector & removeRange(unsigned long imin,unsigned long imax);
		   /*! Resize vector, using brute realloc. You can use it, only if working with POD or structures
			   \param[in] _sz new size 
		   */
		   void  rescale(unsigned long _sz);
};

}




/*! Convenience typedef
*/
#define hlvector hst::vector

#ifndef HI_LVECTOR_H
       #include "hlvector_src.hpp"
#endif
#define HI_LVECTOR_H

#endif

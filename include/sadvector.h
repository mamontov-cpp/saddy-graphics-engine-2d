/*! \file    sadvector.h
    \author  mamontov-cpp

	\brief   Definition of Vector.

	This file contains a definition of Vector.
*/
#pragma once
#include <vector>

namespace sad
{
/*! \class  Vector

	Defines a template class of Vector.
*/
template<class T> class Vector:public std::vector<T>
{
  public:
          Vector();                                        //!< Constructor, that creates an empty Vector.
          /*! Copy constructor.
		      \param[in] h other Vector
		  */
		  Vector(const Vector & h);
          /*! Assignment operator overload
		      \param[in] Sh other Vector
			  \return self-reference
          */
          Vector & operator=(const Vector & Sh);
		  /*! Compares Vectors by length and by elements end displays equiualence
		      \param[in] o other Vector
			  \return true if equiualent
		  */
          bool operator==(const Vector & o);
		  /*! Compares Vectors by length and by elements end displays non-equiualence
		      \param[in] o other Vector
			  \return true if not equiualent
		  */
		  bool operator!=(const Vector & o);
		  virtual ~Vector();                                       //!< Destructor.
          //Геттеры---------------
           inline unsigned long count() const;                               //!< Returns a size of Vector
           inline T*   data()  const;                               //!< Returns a pointer to data
           //----------------------
           /*! Adds object to Vector. Equal to Vector::add
		       \param[in] obj object to be added
			   \return self-reference
		   */
		   Vector & operator<<(const T & obj);
			/*! Appends Vector to Vector. 
		       \param[in] obj Vector to be added
			   \return self-reference
		   */
		   Vector & operator<<(const sad::Vector<T> & obj);
           /*! Copys a last object in Vector, with removing it
		       from Vector
			   \param[in] obj reference, where removed object will be stored
			   \return self-reference
		   */
		   Vector & operator>>(T & obj);
           /*! Adds object to Vector
		       \param[in] obj object
			   \return self-reference
		   */
		   Vector & add(const T & obj);
           /*! Adds object to a front of Vector. Equal to Vector::insert(T,0)
		       \param[in] obj object to be added
			   \return self-reference
		   */
		   Vector & addFront(const T & obj);
           /*! Inserts an object to Vector in specified position.
			   \param obj  Object to add to
			   \param i    Position to add
			   \return self-reference
		   */
		   Vector & insert(const T &obj,unsigned long i);
           /*! Removes an element in specified position
		       \param[in] i position of object, which will be removed
			   \return self-reference
		   */
		   Vector & removeAt(unsigned long i);
           /*! Removes occurences of element  obj
		       \param[in] obj object to be removed
			   \return self-reference
		   */
		   Vector & removeAll(const T & obj);
           /*! Removes first occurence of element  obj
		       \param[in] obj object to be removed
			   \return self-reference
		   */
		   Vector & removeFirst(const T & obj);
           /*! Removes last occurence of element  obj
		       \param[in] obj object to be removed
			   \return self-reference
		   */
		   Vector & removeLast(const T & obj);
           /*! Removes an elements in range from imin to imax
		       \param[in] imin minimum  range index
			   \param[in] imax maximum  range index
			   \return self-reference
		   */
		   Vector & removeRange(unsigned long imin,unsigned long imax);
		   /*! Resize Vector, using brute realloc. You can use it, only if working with POD or structures
			   \param[in] _sz new size 
		   */
		   void  rescale(unsigned long _sz);
};

}

#include "sadvector_src.h"


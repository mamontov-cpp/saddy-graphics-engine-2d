/*! \file    hlvector.hpp
    \author  HiddenSeeker

	\brief   Definition of vector.

	This file contains a definition of vector.
*/
#include <string.h>
#pragma once

#ifndef __H_LINEAR_VECTOR
	#define __H_LINEAR_VECTOR
namespace hst
{
/*! \struct vector_pool
    vector_pool, for growing up some vector
    vector_pool grows with a size of 1.5
*/
template<typename T>
struct vector_pool
{
  T * p;                        //!< vector_pool data pointer
  unsigned long sz;             //!< Current size of vector_pool
};

/*! Grows a vector_pool in 2
    \param[in] _pool vector_pool to grow
*/
template<typename T>
void pool_growup(vector_pool<T> * _pool);
/*! Shrinks a vector_pool
    \param[in] _pool vector_pool to shrink
	\param[in] newsz new size of pool
*/
template<typename T>
void pool_shrink(vector_pool<T> * _pool,unsigned long newsz);
/*! Destroys a vector_pool
    \param[in]  _pool vector_pool to destroy
*/
template<typename T>
void pool_destroy(vector_pool<T> * _pool);
/*! \class  vector
    \brief  Vector

	Defines a template class of vector.
*/
template<class T> class vector
{
  private:
          unsigned long sz;                                           //!< Size of vector.


          vector_pool<T> m_pool;                                                //!< vector_pool


          /*! Cleanups the vector.
		  */
		  void makeClear();
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
		   /*! Debug print
		       \param[in] cp - format string in printf() format
		   */
           void dbg_ass(const char * cp);
           /*! Returns reference to specified element. Returns a
		       reference to default
		       element, if element cannot be found.
			   \param[in] i index of object
			   \return reference to object
		   */
		   inline T&   operator[](unsigned long i);
           /*! Returns constant reference to specified element. Returns a
		       reference to default
		       element, if element cannot be found.
			   \param[in] i index of object
			   \return constant reference to object
		   */
		   inline const T &   operator[](unsigned long i) const;
           //----------------------
           /*! Adds object to vector. Equal to vector::add
		       \param[in] obj object to be added
			   \return self-reference
		   */
		   vector & operator<<(const T & obj);
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
		   vector & remove(const T & obj);
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
           /*! Cleanups vector, removing all elements
		   */
		   void		clear();
		   /*! Maps function to every element in vector
		       \param[in] fptr pointer to mapping function
		   */
		   void     yield(void (*fptr)(T & obj));
		   /*! Resize vector, using brute realloc. You can use it, only if working with POD or structures
			   \param[in] _sz new size 
		   */
		   void  rescale(unsigned long _sz);
		   /*!  Pushes into back data
				\param[in] obj object
				\return object data
		    */
		   hst::vector<T> & push_back(const T & obj);
};

}


/*! Operator new for vector_pool
    \param[in] sz      size of  vector_pool
    \param[in,out]     _pool     vector_pool
    \param[in]         allocated current allocated elements of vector
    \param[in]        where where he must be constructed
    \return new address
*/
template<typename T>
void * operator new(size_t sz, hst::vector_pool<T> *  _pool, unsigned long allocated,unsigned long where);
/*! Operator delete for vector_pool,none needed,just to prevent warnings
    \param[in] pp      deleting pointer
    \param[in,out]     _pool     vector_pool
    \param[in]         allocated current allocated elements of vector
    \param[in]        where where he must be constructed
    \return new address
*/
template<typename T>
void  operator delete(void * pp, hst::vector_pool<T> *  _pool, unsigned long allocated,unsigned long where);



/*! Convenience typedef
*/
#define hlvector hst::vector

#ifndef HI_LVECTOR_H
       #include "hlvector_src.hpp"
#endif
#define HI_LVECTOR_H

#endif

/*! \file    sadvector.h

    \brief   Definition of basic vector class.

    This file contains a definition of vector.
*/
#pragma once
#include <cstdlib>
#include <vector>

namespace sad
{
/*! \class  Vector

    Defines a template class of Vector.
*/
template<
    typename T
> 
class Vector: public std::vector<T>
{
public:
    Vector();
    inline Vector(std::initializer_list<T> init) : std::vector<T>(init)
    {

    }
    virtual ~Vector();                                       //!< Destructor.
    inline size_t count() const;                             //!< Returns a size of Vector
    inline T*   data()  const;                               //!< Returns a pointer to data
    /*! Adds object to sad::Vector<T>. Equal to sad::Vector<T>::add
        \param[in] obj object to be added
        \return self-reference
     */
    sad::Vector<T> & operator<<(const T & obj);
    /*! Appends sad::Vector<T> to sad::Vector<T>. 
        \param[in] obj sad::Vector<T> to be added
        \return self-reference
     */
    sad::Vector<T> & operator<<(const sad::Vector<T> & obj);
    /*! Performs copying for a last object in sad::Vector<T>, removing it 
        \param[in] obj reference, where removed object will be stored
        \return self-reference
     */
    sad::Vector<T> & operator>>(T & obj);
    /*! Adds object to sad::Vector<T>
        \param[in] obj object
        \return self-reference
     */
    sad::Vector<T> & add(const T & obj);
    /*! Adds object to a front of sad::Vector<T>. Equal to sad::Vector<T>::insert(T,0)
        \param[in] obj object to be added
        \return self-reference
     */
    sad::Vector<T> & addFront(const T & obj);
    /*! Inserts an object to sad::Vector<T> in specified position.
        \param obj  Object to add to
        \param i    Position to add
        \return self-reference
     */
    sad::Vector<T> & insert(const T &obj,unsigned long i);
    /*! Removes an element in specified position
        \param[in] i position of object, which will be removed
        \return self-reference
     */
    sad::Vector<T> & removeAt(unsigned long i);
    /*! Removes occurrences of element  obj
        \param[in] obj object to be removed
        \return self-reference
     */
    sad::Vector<T> & removeAll(const T & obj);
    /*! Removes first occurrence of element  obj
        \param[in] obj object to be removed
        \return self-reference
     */
    sad::Vector<T> & removeFirst(const T & obj);
    /*! Removes last occurrence of element  obj
        \param[in] obj object to be removed
        \return self-reference
     */
    sad::Vector<T> & removeLast(const T & obj);
    /*! Removes an elements in range from i_min to i_max
        \param[in] i_min minimum  range index
        \param[in] i_max maximum  range index
        \return self-reference
     */
    sad::Vector<T> & removeRange(unsigned long i_min,unsigned long i_max);
};

}

#include "sadvector_src.h"


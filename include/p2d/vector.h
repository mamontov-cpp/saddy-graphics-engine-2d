/*! \file vector.h
    

    Defines a vector as a special case of point
 */
#pragma once
#include "../sadpoint.h"

namespace sad
{

namespace p2d 
{
    /*! A vector is defined by two coordinated x and y
     */
    typedef sad::Point2D Vector;
    /*!  Modulo of vector
         \param[in] v vector
         \return modulo
     */
    double modulo(const p2d::Vector & v);
    /*! Returns a default vector as (1/2^0.5,1/2^0.5)
        \return a default vector data
     */
    sad::p2d::Vector basis();	
    /*! Computes unit-vector for vector
        \param[in] v vector
        \return vector
     */
    sad::p2d::Vector unit(const sad::p2d::Vector & v);
    /*! Computes unit vector and replace value of vector with it's unit vector
        \param[in, out] v vector
     */
    void mutableUnit(sad::p2d::Vector & v);
    /*! A class for orthogonal vector index, used to
        restrict range of optional vector index
     */
    enum class OrthoVectorIndex : int
    {
        OVI_DEG_90  = 0,  //!< An orthogonal vector as a rotation to 90 degrees
        OVI_DEG_270 = 1   //!< An orthogonal vector as a rotation to 270 degrees
    };
    /*! Orthogonal vector
        \param[in] v vector
        \param[in] i index
        \return a vector, corresponding to value
     */
    sad::p2d::Vector ortho(const sad::p2d::Vector & v, sad::p2d::OrthoVectorIndex i);
    /*! Orthogonal vector to normalized vector
        \param[in] v vector
        \param[in] i index
        \return a vector, corresponding to value
     */
    void mutableNormalizedOrtho(sad::p2d::Vector & v, sad::p2d::OrthoVectorIndex i);
    /*! Scalar multiplication of a vector
        \param[in] v1 first vector
        \param[in] v2 second vector
     */
    double scalar(const sad::p2d::Vector & v1, const sad::p2d::Vector & v2);
}

}

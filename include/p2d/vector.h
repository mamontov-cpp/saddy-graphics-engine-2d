/*! \file vector.h
	\author HiddenSeeker

	Defines a vector as a special case of point
 */
#pragma once
#include "../primitives/hpoint.h"

namespace p2d 
{
	/*! A vector is defined by two coordinated x and y
	 */
	typedef hPointF Vector;
	/*!  Modulo of vector
		 \param[in] v vector
		 \return modulo
	 */
	double modulo(const p2d::Vector & v);
	/*! Returns a default vector as (1/2^0.5,1/2^0.5)
		\return a default vector data
	 */
	p2d::Vector basis();	
	/*! Computes unit-vector for vector
		\param[in] v vector
		\return vector
	 */
	p2d::Vector unit(const p2d::Vector & v);
	/*! A class for orthogonal vector index, used to
		restrict range of optional vector index
	 */
	enum OrthoVectorIndex 
	{
		OVI_DEG_90,   //!< An orthogonal vector as a rotation to 90 degrees
		OVI_DEG_270   //!< An orthogonal vector as a rotation to 270 degrees
	};
	/*! Orthogonal vector
		\param[in] v vector
		\param[in] i index
		\return a vector, corresponding to value
	 */
	p2d::Vector ortho(const p2d::Vector & v, OrthoVectorIndex i);
	/*! Scalar multiplication of a vector
		\param[in] v1 first vector
		\param[in] v2 second vector
	 */
	double scalar(const p2d::Vector & v1, const p2d::Vector & v2);
}


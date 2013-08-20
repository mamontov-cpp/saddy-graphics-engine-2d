/*! \file tangentialforce.h
	\author HiddenSeeker

	Describes a tangential force, acting on object
 */
#pragma once
#include "tickable.h"

namespace p2d
{

/*! A class for getting default value for a tickable state
 */
template<>
class TickableDefaultValue<double>
{
 public:
	 static inline double get() { return 0; }
	 static inline double zero() { return 0; }
};

/*! A force as a tickable state
 */
typedef p2d::TickableState<double> TangentialForce;
/*! A flow for a force
 */
typedef p2d::TickableFlow<double> TangentialForceFlow;


}


/*! \file force.h
	\author HiddenSeeker

	Describes a force, acting on object
 */
#pragma once
#include <p2d/vector.h>
#include "../templates/maybe.hpp"
#include "tickable.h"


namespace p2d
{
/*! A class for getting default value for a tickable state
 */
template<>
class TickableDefaultValue<p2d::Vector>
{
 public:
	 static inline p2d::Vector get() { return p2d::Vector(0, 0); }
	 static inline p2d::Vector zero() { return p2d::Vector(0, 0); }
};

/*! A force as a tickable state
 */
typedef p2d::TickableState<p2d::Vector> Force;
/*! A flow for a force
 */
typedef p2d::TickableFlow<p2d::Vector> ForceFlow;

}

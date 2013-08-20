/*! \file ghostoptions.h
	\author HiddenSeeker

	Describes an options for object, that defines it as a ghost, i.e.
	an object, which does not collide with any other objects
 */
#pragma once
#include "tickable.h"

namespace p2d
{

/*! A class for getting default value for a tickable state
 */
template<>
class TickableDefaultValue<bool>
{
 public:
	 static inline bool get() { return false; }
};


/*! A ghost options as a tickable state
 */
typedef p2d::TickableState<bool> GhostOptions;
/*! A flow for a ghost options
 */
typedef p2d::TickableFlow<bool> GhostOptionsFlow;

}



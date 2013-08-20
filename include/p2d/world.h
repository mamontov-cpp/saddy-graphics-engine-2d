/*! \file world.h
	\author HiddenSeeker

	Defines world simulation as set of items and callbacks
 */
#pragma once

namespace p2d
{
/*! A world is a set if simulated items, and callbacks used to define
	behaviour between them
 */
class World
{
 private:
	 /*! Current time step
	  */
	 double m_time_step;
 public:
	 /*! Returns current time step for a world
		 \return a time step for a world
	  */
	 double timeStep() const;
};

}


/*! \file world.h
	\author HiddenSeeker

	Defines world simulation as set of items and callbacks
 */
#pragma once
#include "circletohulltransformer.h"
#include "collisiondetector.h"
#include "simplecollisiondetector.h"
#include "broadcollisiondetector.h"
#include "multisamplingcollisiondetector.h"

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
	 /*! A common transformer for all shapes
	  */
	 p2d::CircleToHullTransformer * m_transformer;
	 /*! A collision dispatcher for testing an items for collision
	  */
	 p2d::CollisionDetector * m_detector;
 public:
	 /*! Creates world with default transformer
	  */
	 World();
	 /*! Destroys world
	  */
	 ~World();
	 /*! Returns a transformer for all circles
		 \return a transformer for all circles
	  */
	 p2d::CircleToHullTransformer * transformer();
	 /*! Returns current time step for a world
		 \return a time step for a world
	  */
	 double timeStep() const;
};

}


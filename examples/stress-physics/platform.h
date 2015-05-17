/*! \file platform.h
	\author HiddenSeeker

	Describes a ball
 */
#include <p2d/app/object.h>
#include <p2d/app/constants.h>
#pragma once

/*! As a default, grid node is a node
 */
class Platform: public sad::p2d::app::Object
{
 SAD_OBJECT
 public:
	 Platform();
};

/*! \file gridnode.h
	\author HiddenSeeker

	Describes a basic grid node
 */
#include <p2d/app/object.h>
#include <p2d/app/constants.h>
#pragma once

/*! As a default, grid node is a node
 */
class GridNode: public sad::p2d::app::Object
{
 SAD_OBJECT
 public:
	 GridNode();
};

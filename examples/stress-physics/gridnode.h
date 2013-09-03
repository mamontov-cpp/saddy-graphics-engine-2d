/*! \file gridnode.h
	\author HiddenSeeker

	Describes a basic grid node
 */
#include "worldobject.h"
#include "worldobjectconstants.h"
#pragma once

/*! As a default, grid node is a node
 */
class GridNode: public WorldObject
{
 SAD_OBJECT
 public:
	 GridNode();
};

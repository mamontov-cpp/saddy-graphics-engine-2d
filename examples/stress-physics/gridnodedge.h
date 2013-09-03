/*! \file gridnodeedge.h
	\author HiddenSeeker

	Describes a basic grid node edge
 */
#include "worldobject.h"
#include "worldobjectconstants.h"
#include <sprite2dadapter.h>
#pragma once

/*! As a default, grid node is a node
 */
class GridNodeEdge: public sad::BasicNode
{
 SAD_OBJECT
 protected:
	 p2d::Body * m_b1;
	 p2d::Body * m_b2;
	 Sprite2DAdapter * m_ad;
 public:
	 GridNodeEdge(p2d::Body * b1, p2d::Body * b2);

	 virtual void render();
};

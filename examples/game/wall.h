/*! \file wall.h
	\author HiddenSeeker

	Describes a bounding walls, that teleports collided player to other wall
	and destroy other objects
 */
#include <primitives/object.h>
#include <p2d/body.h>
#include <p2d/circle.h>
#pragma once

/*! Specifies a wall type
 */
enum WallType
{
	WT_LEFT = 0,
	WT_RIGHT = 1,
	WT_UP = 2,
	WT_DOWN = 3
};

class Player;
class Wall: public sad::Object
{
  SAD_OBJECT
  protected:
	  /*! A type of wall
	   */
	  WallType m_type;
	  /*! Returns a teleportation position for circle
		  \param[in] c circle
	   */
	  p2d::Point position(p2d::Circle * c);
  public:
	  /*! A type of wall
		  \param[in] w wall type
	   */
	  Wall(WallType w);
	  /*! Tries to teleport an object
	   */
	  void tryTeleport(Player * p);
};

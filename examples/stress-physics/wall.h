/*! \file wall.h
	\author HiddenSeeker

	Describes a bounding walls, that teleports collided player to other wall
	and destroy other objects
 */
#include <primitives/object.h>
#include <p2d/body.h>
#include <p2d/circle.h>
#include <p2d/line.h>
#include <p2d/bounds.h>
#pragma once

class Player;
class Wall: public sad::Object
{
  SAD_OBJECT
  protected:
	  /*! A type of wall
	   */
	  p2d::BoundType m_type;
	  /*! Returns a teleportation position for circle
		  \param[in] c circle
	   */
	  p2d::Point position(const p2d::Point & ce);
  public:
	  /*! A type of wall
		  \param[in] w wall type
	   */
	  Wall(p2d::BoundType w);
	  /*! Tries to teleport an object
	   */
	  void tryTeleport(p2d::Body * b);
};


/*! Describes walls as in-game bounding container, when object
	within walls behaves normally, and when out of walls - it'll be destroyed
	or teleported (if this is player)
 */
class Walls
{
  protected:
	  hst::vector<Wall *> m_walls;  //!< A bounding walls
	  hst::vector<p2d::Body *> m_bodies; //!< A bodies
  public:
	  /*! A minimal definition for a wall
	   */
	  typedef hst::pair<p2d::BoundType, double> minimal_t;
	  /*! Creates walls and bodies
	   */
	  Walls();
	  /*! Returns physical bodies of walls to add them into a game world
	   */
	  const hst::vector<p2d::Body *> bodies() const;
      /*! Destroys walls
	   */
	  ~Walls();
};

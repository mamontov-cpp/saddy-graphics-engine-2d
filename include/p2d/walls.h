/*! \file walls.h
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
/*! Describes a static non-moving wall
 */
class Wall: public sad::Object
{
  SAD_OBJECT
  protected:
	  /*! A padding for opposite wall
	   */
	  double m_padding;
	  /*! A type of wall
	   */
	  p2d::Body  * m_opposite_body;
	  /*! A body
	   */
	  p2d::Body * m_body;
  public:
	  /*! A type of wall
		  \param[in] paddign a padding
	   */
	  Wall(double padding = 0.0);
	  /*! Tries to teleport an object
		  \param[in] p body
	   */
	  void tryTeleport(p2d::Body * p);
	  /*! Returns a body
		  \return body
	   */
	  inline p2d::Body * body() { return m_body; }
	  /*! Sets new body
		  \param[in] b body
	   */
	  inline void setBody(p2d::Body * b) { m_body = b; }
	  /*! Returns body for opposite wall
		  \return opposite body
	   */
	  inline p2d::Body * oppositeBody() { return m_body; }
	  /*! Sets body for opposite wall
		  \param[in] b opposite body
	   */
	  inline void setOppositeBody(p2d::Body * b) {m_opposite_body = b;} 
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

	  double m_padding; //!< A padding for walls and bodies
  public:
	  /*! A minimal definition for a wall
	   */
	  typedef hst::pair<p2d::BoundType, double> minimal_t;
	  /*! Creates bounding walls with specified padding
		  \param[in] padding
	   */
	  Walls(double padding);
	  /*! Returns physical bodies of walls to add them into a game world
	   */
	  const hst::vector<p2d::Body *> bodies() const;
      /*! Destroys walls
	   */
	  virtual ~Walls();
};

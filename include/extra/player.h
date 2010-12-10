/*! \file   player.h
    \author HiddenSeeker
	\brief class Player is placed here

	Here is placed class of player - class, that provides any player associated
	objects
*/
#include "movitem.h"
#pragma once

class Player: public Collidable
{
 private:
	     sad::Texture * m_tex; //!< Associated texture
		 BoundingBox m_box; //!< Bounding box
		 BoundingBox m_draw; //!< Draw box
		 float       m_angle;   //!< Angle
 public:

	     static int      Type;      //!<  Global type id
		 static Player * instance; //!< Player instance
	     /*! Constructs player
		     \param[in] box     box place
			 \param[in] percent percent place
		 */
	     Player(const BoundingBox & box,float percent);

		 const BoundingBox & prect();

		 BoundingBox rect();

		 void render();

		 /*! Sets an angle
		 */
		 void setAngle(float angle);
		 /*! Moves a player in specified vector
		 */
		 void move(const Vector & p);
		 /*! Shoots
		 */
		 void shoot();

		 ~Player();
};

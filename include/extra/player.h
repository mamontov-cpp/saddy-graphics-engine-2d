/*! \file   player.h
    \author HiddenSeeker
	\brief class Player is placed here

	Here is placed class of player - class, that provides any player associated
	objects
*/
#include "movitem.h"
#include "collisionmanager.h"
#include "../texturemanager.h"
#pragma once

typedef hPointF Vector;

class Player: public Collidable
{
 SAD_NODE
 private:
		 bool        m_first_render; //!< First rendering
		 float       m_angle;   //!< Angle
		 float       m_velocity[2]; //!< Player velocity
		 clock_t     m_lastshot;    //!< Time,when last shot was performed
 public:
	     inline void toggleVelocityX(float x) {
		 m_velocity[0]=x;
		 }
		 inline void toggleVelocityY(float y) {
		 m_velocity[1]=y;
		 }
		 /*! Constructs player
		     \param[in] pos     position
		 */
	     Player(const hPointF & pos);


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


typedef Instance<Player> PlayerInstance;
/*! \file movitem.h
    \author DVK, HiddenSeeker

	Describes a movable items

	A movable item, is an item, that moves in one direction, until reaches
	an end of screen, or some other condition. Also it rotates. 
*/
#include "../renderer.h"
#include <math.h>
#include "statemachine.h"
#include "rigid_body.h"
#include "collisionmanager.h"
#include "../texturemanager.h"
#pragma once

typedef hPointF Vector;

class PlayerBullet: public Collidable 
{
 SAD_NODE
 public:
        /*! Creates an item
		    \param[in] vec direction, that will be reached in second
		    \param[in] pos position, where it's being drawn
	    */
	    PlayerBullet(
		             const Vector & vec,
		             const hPointF &  pos 
			        );
		~PlayerBullet();
};


class EnemyBullet: public Collidable 
{
 SAD_NODE
 public:
        /*! Creates an item
		    \param[in] vec direction, that will be reached in second
		    \param[in] pos position, where it's being drawn
	     */
	    EnemyBullet(
		             const Vector & vec,
		             const hPointF & pos
			        );
		~EnemyBullet();
};

class Bonus: public Collidable
{
 SAD_NODE
 public:
        /*! Creates an item
		    \param[in] vec direction, that will be reached in second
		    \param[in] pos position
	     */
	    Bonus(
		      const Vector & vec,
		      const hPointF &  pos
			 );
		~Bonus();
};


class Enemy: public Collidable 
{
 SAD_NODE
 public:
        /*! Creates an item
		    \param[in] vec direction, that will be reached in second
		    \param[in] pos position
	     */
	    Enemy(
		             const Vector & vec,
		             const hPointF &  pos 
			        );
		~Enemy();
};

#define SHOOT_FREQ 1000
class ShootingEnemy: public Collidable 
{
 SAD_NODE
 private:
	     clock_t m_lastclock;
 public:
        /*! Creates an item
		    \param[in] vec direction, that will be reached in second
	     */
	    ShootingEnemy(
		             const Vector & vec,
		             const hPointF &  pos 
			        );
		/*! Renders an enemy
		*/
		void render();
		~ShootingEnemy();
};

#define SPAWN_FREQ 350
#define SPAWN_FREQ4 200
#define IDLE_RAIN  1
#define REAL_SPAWN 2
#define BOUND_X1 0.0f
#define BOUND_X2 640.0f
#define BOUND_Y1 0.0f
#define BOUND_Y2 480.0f
/*! Class, that periodicly emits some enemies
*/
class EnemyEmitter:public sad::BasicNode
{
 SAD_NODE
 private:
	     clock_t m_clk;
		 void (EnemyEmitter::*m_r)();
 public:
	     EnemyEmitter(int what=REAL_SPAWN);
		 void render();
		 void renderRain();
		 void renderSpawn();
		 ~EnemyEmitter();
};


/*! \file movitem.h
    \author DVK, HiddenSeeker

	Describes a movable items

	A movable item, is an item, that moves in one direction, until reaches
	an end of screen, or some other condition. Also it rotates. 
*/
#include "types.h"
#include "renderer.h"
#include <math.h>
#include "collisionmanager.h"
#include "texturemanager.h"
#pragma once

#define ZAX 0.5

typedef hPointF Size; //!< Size is a point, where on x placed width, on y - height
typedef hPointF Vector; //!< vector of direction
class MovingItem: public Collidable 
{
protected:
		   BoundingBox m_box; //!< Bounding box
		   BoundingBox m_draw; //!< Draw box
		   Vector      m_direct; //!< Direction
		   float       m_angle;   //!< Angle
		   sad::Texture * m_tex; //!< Texture

		   void move();
public:
    /*! Creates an item
	    \param[in] tex current texture
		\param[in] vec direction, that will be reached in second
		\param[in] draw     bounding box, of drawed iterm
		\param[in] percent  (real bounding box)/(drawed bounding box)
	*/
	MovingItem(
		       sad::Texture * tex,
		       const Vector & vec,
		       const BoundingBox &  draw, 
			   float percent
			  );
	BoundingBox rect();
    virtual void render();

	virtual ~MovingItem();
};


class PlayerBullet: public MovingItem 
{
 public:
	    static int Type;
        /*! Creates an item
		    \param[in] vec direction, that will be reached in second
		    \param[in] draw     bounding box, of drawed iterm
		    \param[in] percent  (real bounding box)/(drawed bounding box)
	     */
	    PlayerBullet(
		             const Vector & vec,
		             const BoundingBox &  draw, 
			         float percent
			        );
		~PlayerBullet();
};


class EnemyBullet: public MovingItem 
{
 public:
	    static int Type;
        /*! Creates an item
		    \param[in] vec direction, that will be reached in second
		    \param[in] draw     bounding box, of drawed iterm
		    \param[in] percent  (real bounding box)/(drawed bounding box)
	     */
	    EnemyBullet(
		             const Vector & vec,
		             const BoundingBox &  draw, 
			         float percent
			        );
		~EnemyBullet();
};

class Bonus: public MovingItem 
{
 public:
	    static int Type;
        /*! Creates an item
		    \param[in] vec direction, that will be reached in second
		    \param[in] draw     bounding box, of drawed iterm
		    \param[in] percent  (real bounding box)/(drawed bounding box)
	     */
	    Bonus(
		             const Vector & vec,
		             const BoundingBox &  draw, 
			         float percent
			        );
		~Bonus();
};


class Enemy: public MovingItem 
{
 public:
	    static int Type;
        /*! Creates an item
		    \param[in] vec direction, that will be reached in second
		    \param[in] draw     bounding box, of drawed iterm
		    \param[in] percent  (real bounding box)/(drawed bounding box)
	     */
	    Enemy(
		             const Vector & vec,
		             const BoundingBox &  draw, 
			         float percent
			        );
		~Enemy();
};

#define SHOOT_FREQ 1000
class ShootingEnemy: public MovingItem 
{
 private:
	     clock_t m_lastclock;
 public:
	    static int Type;
        /*! Creates an item
		    \param[in] vec direction, that will be reached in second
		    \param[in] draw     bounding box, of drawed iterm
		    \param[in] percent  (real bounding box)/(drawed bounding box)
	     */
	    ShootingEnemy(
		             const Vector & vec,
		             const BoundingBox &  draw, 
			         float percent
			        );
		/*! Renders an enemy
		*/
		void render();
		~ShootingEnemy();
};

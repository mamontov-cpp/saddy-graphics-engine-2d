/*! \file gameobject.h

	Describes main game object, that is basic for all objects in game
 */
#pragma once
#include "p2d/body.h"
#include "sprite2dadapter.h"
#include "primitives/object.h"

class Game;
/*! Describes a basic in-game object, which provides primitives, needed to 
	describe all in-game objects
 */
class GameObject: public sad::BasicNode
{
	/* Declare metadata, needed to describe inheritance tree, name of class
	   This metadata can be used where real type is needed - for most part,
	   for collision detection
	 */
	SAD_OBJECT
 protected:
	 /*! A body is representation for a game object in physics engine, needed
	  */
	 p2d::Body * m_body;
	 /*! A sprite is a graphical representation for a game object
	  */
	 Sprite2DAdapter * m_sprite;
	 /*! An object is linked to game to forward the in-game to a game object 
	  */
	 Game * m_game;
	 /*! Defines amount of times, which game object can be hit (one by default)
	  */
	 int m_hp;
 protected:
	 /*! Inits game object parameters from constants of specified type
	  */
	 template<typename T> void initFromConstants()
	 {
		 Sprite2DAdapter::Options * o = GameObjectConstants<T>::sprite();
		 this->m_sprite->set(*o);
		 delete o;
		 this->m_body->setShape(GameObjectConstants<T>::shape());
	 }
 public:
	 /*! Creates an empty game object
	  */
	 GameObject();
	 /*! Moves an object sprite by specified direction.
	     Note, that this function should be called only from body's listener,
		 to preserve synchronization between sprite and physical body
		 \param[in] dist distance 
	  */
	 void notifyMove(const p2d::Vector & dist);
	 /*! Rotates an object sprite nby specified direction
		 Note, that this function should be called only from body's listener
		 to preserve synchronization between sprite and physical body
		 \param[in] angle rotated angle
	  */
	 void notifyRotate(const double & angle);
	 /*! Sets an angle of game object to specified
		 \param[in] angle an angle
	  */
	 void setAngle(double angle);
	 /*! Makes game object look to specified point
		 \param[in] p point
	  */
	 void lookAt(const hPointF & p);
	 /*! Sets angular velocity, making object rotating clockwise
		 \param[in] v velocity
	  */
	 void setAngularVelocity(double v);
	 /*! Sets only horizontal speed
		 \param[in] v speed
	  */
	 void setHorizontalSpeed(double v);
	 /*! Sets only vertical speed
		 \param[in] v speed
	  */
	 void setVerticalSpeed(double v);
	 /*! Stops horizontal movement
	  */
	 void stopHorizontal();
	 /*! Stops vertical movement
	  */
	 void stopVertical();
	 /*! Stops a game object tangential movement
	  */
	 void stop();
	 /*! Sets a game, which object belongs to
		 \param[in] g game
	  */
	 void setGame(Game * g);
	 /*! The object does not own anything, if game is not null
	  */
	 ~GameObject();
	 /*! Returns body of game object	
	     \return body of game object
	  */ 
	 p2d::Body * body();
	 /*! Teleports a game object to specified point now
		 \param[in] p new center of game object
	  */
	 void teleportNow(const p2d::Point & p);
	 /*! Teleports a game object on next iteration at start of next iteration
		 \param[in] p new center of game object
	  */
	 void teleportLater(const p2d::Point & p);
	 /*! Returns remaining hit points
	  */
	 int hitPoints() const;
	 /*! Increments hit points, by specified count
		 \param[in] count how much hit points should be incremented
	  */
	 void incrementHP(int count);
	 /*! Decrements hit points, by specified count.
	     If hit points became less or equal to zero, game object is removed
		 \param[in] count how much
	  */ 
	 void decrementHP(int count);
	 /*! Called, when object is rendered. Object can easily reimplement
		 it to work with AI, or do something other (like shoot). 
	  */
	 virtual void render();
};


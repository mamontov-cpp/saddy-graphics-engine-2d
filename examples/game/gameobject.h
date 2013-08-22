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
 private:
	 /*! A body is representation for a game object in physics engine, needed
	  */
	 p2d::Body * m_body;
	 /*! A sprite is a graphical representation for a game object
	  */
	 Sprite2DAdapter * m_sprite;
	 /*! An object is linked to game to forward the in-game to a game object 
	  */
	 Game * m_game;
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
	 /*! Sets a game, which object belongs to
		 \param[in] g game
	  */
	 void setGame(Game * g);
	 /*! The object does not own anything, if game is not null
	  */
	 ~GameObject();
	 /*! Called, when object is rendered. Object can easily reimplement
		 it to work with AI, or do something other (like shoot). 
	  */
	 virtual void render();
};


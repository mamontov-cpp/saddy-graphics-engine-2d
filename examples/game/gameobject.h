/*! \file gameobject.h

    Describes main game object, that is basic for all objects in game
 */
#pragma once
#include "abstractautomaticgun.h"

#include <phy52d/object.h>


class Game;
/*! Describes a basic in-game object, which provides primitives, needed to 
    describe all in-game objects
 */
class GameObject: public sad::phy52d::Object
{
    /* Declare metadata, needed to describe inheritance tree, name of class
       This metadata can be used where real type is needed - for most part,
       for collision detection
     */
    SAD_OBJECT
 protected:
     /*! Defines amount of times, which game object can be hit (one by default)
      */
     int m_hp;
     /*! An inner guns for object
      */
     sad::Vector<AbstractAutomaticGun * > m_guns;
 public:
     /*! Creates an empty game object
      */
     GameObject();
     /*! The object does not own anything, if game is not nullptr
      */
     virtual ~GameObject() override;
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
     /*! Called, when object is rendered. Object can easily re-implement
         it to work with AI, or do something other (like shoot). 
      */
     virtual void render() override;
     /*! Returns a game from game object
         \return game
      */
     Game * game() const;
     /*! Adds a new gun for object
         \param[in] gun a gun
      */
     void addGun(AbstractAutomaticGun * gun);
};


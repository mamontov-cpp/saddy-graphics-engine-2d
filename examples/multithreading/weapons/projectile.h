/*! \file projectile.h
 
    A projectile, that should be stored in game
 */
#pragma once
#include <object.h>

#include "../game/actor.h"

namespace weapons
{
/*! A projectile that will be stored in game
 */
class Projectile: public sad::Object
{
    SAD_OBJECT
    /*! Must be inherited
     */
    virtual ~Projectile();
    /*! Returns actor
        \return actor
     */
    virtual game::Actor* actor() const = 0;
    /*! Kills an object
     */
    virtual void kill()  = 0;
    /*! Updates side for a swing
     */
    virtual void update() = 0;
};

}


DECLARE_TYPE_AS_SAD_OBJECT_ENUM(weapons::Projectile);
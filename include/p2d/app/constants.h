/*! \file constants.h
    

    Describes  object  constants, used to initialize sprites for
    various objects.

    Object constants contain various rendering parameters, such as
    name of texture, texture coordinates and size of polygon, which is
    texture and texture coordinates mapped to.

    Also they contain collision shape templates.
 */
#pragma once

#include "../circle.h"
#include "../line.h"
#include "../rectangle.h"

#include "../../sprite2d.h"


namespace sad
{

namespace p2d
{

namespace app
{

/*! Main class for getting a sprite constants.

    Constants contain various rendering parameters, such as
    name of texture, texture coordinates and size of polygon, which is
    texture and texture coordinates mapped to.

    For most parts, this class defined mapping of game object type T to sprite
    options, related to him, so we define it for all game object type we have.
 */
template<
    typename T
>
class Constants
{
public:
    /*! Returns a sprite options, needed to create sprite
        \return sprite options
     */
    static Sprite2D::Options * sprite();
    /*! Returns shape needed for physical engine
        \return a shape
     */
    static p2d::CollisionShape * shape();
    /*! Describes a velocity for shooting enemy as a bullet
        \return velocity
     */
    static double velocity();
    /*! Returns a shooting interval for this enemy as a bullet
        \return interval
     */
    static double interval();
};

/*! A dynamically stored constants as a simple container
    Works same, as Constants
 */
class DynamicConstants
{
public:
    Sprite2D::Options * Options;
    p2d::CollisionShape * Shape;
    double Velocity;
    double Interval;

    /*! Clones an options
        \return options clone
     */
    inline Sprite2D::Options * options()  const
    { 
        return new Sprite2D::Options(*(this->Options)); 
    }
    /*! Clones a shape
        \returns a shape clone
     */
    inline p2d::CollisionShape * shape() const
    {
        return Shape->clone();
    }

    inline double velocity() const
    {
        return Velocity;
    }

    inline double interval() const
    {
        return Interval;
    }

    inline DynamicConstants()
    {
        this->Options = nullptr;
        this->Shape = nullptr;
        Velocity = 0;
        Interval = 0;
    }

    /*! Inits constants from static constants
     */
    template<typename T>
    void initFromStatic()
    {
        this->Options = typename p2d::app::Constants<T>::sprite();
        this->Shape = typename  p2d::app::Constants<T>::shape();
        this->Velocity = typename  p2d::app::Constants<T>::velocity();
        this->Interval = typename  p2d::app::Constants<T>::interval();
    }

    inline ~DynamicConstants()
    {
        delete this->Options;
        delete this->Shape;
    }

};

}

}

}

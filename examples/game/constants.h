/*! \file constants.h
    

    Describes game object  constants, used to initialize sprites for
    various game objects.

    Game object constants contain various rendering parameters, such as
    name of texture, texture coordinates and size of polygon, which is
    texture and texture coordinates mapped to.

    Also they contain shape templates.
 */
#pragma once
#include <sprite2d.h>
#include <p2d/body.h>

class Player;
class PlayerBullet;
class EnemyBullet;
class Bonus;
class Enemy;
class ShootingEnemy;
class SuperShootingEnemy;

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
    static sad::Sprite2D::Options * sprite();
    /*! Returns shape needed for physical engine
        \return a shape
     */
    static sad::p2d::CollisionShape * shape();
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
    sad::Sprite2D::Options * Options;
    sad::p2d::CollisionShape * Shape;
    double Velocity;
    double Interval;

    /*! Clones an options
        \return options clone
     */
    inline sad::Sprite2D::Options * options()  const
    { 
        return new sad::Sprite2D::Options(*(this->Options));
    }
    /*! Clones a shape
        \returns a shape clone
     */
    inline sad::p2d::CollisionShape * shape() const
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

    /*! Initializes constants from static constants
     */
    template<typename T>
    void initFromStatic()
    {
        this->Options = Constants<T>::sprite();
        this->Shape = Constants<T>::shape();
        this->Velocity = Constants<T>::velocity();
        this->Interval = Constants<T>::interval();
    }

    inline ~DynamicConstants()
    {
        delete this->Options;
        delete this->Shape;
    }

};


template<>
class Constants<Player>
{
public:
    static sad::Sprite2D::Options * sprite();
    static sad::p2d::CollisionShape * shape();
};



template<>
class Constants<PlayerBullet>
{
public:
    static sad::Sprite2D::Options * sprite();
    static sad::p2d::CollisionShape * shape();
    /*! Describes a velocity for bullet
        \return velocity
     */
    static double velocity();
    /*! Returns a shooting interval with this bullet type
     */
    static double interval();
};


template<>
class Constants<EnemyBullet>
{
public:
    static sad::Sprite2D::Options * sprite();
    static sad::p2d::CollisionShape * shape();
    /*! Describes a velocity for bullet
        \return velocity
     */
    static double velocity();
    /*! Returns a shooting interval with this bullet type
     */
    static double interval();
};


template<>
class Constants<Bonus>
{
public:
    static sad::Sprite2D::Options * sprite();
    static sad::p2d::CollisionShape * shape();
};


template<>
class Constants<Enemy>
{
public:
    static sad::Sprite2D::Options * sprite();
    static sad::p2d::CollisionShape * shape();
};


template<>
class Constants<ShootingEnemy>
{
public:
    static sad::Sprite2D::Options * sprite();
    static sad::p2d::CollisionShape * shape();
    /*! Describes a velocity for shooting enemy as a bullet
        \return velocity
     */
    static double velocity();
    /*! Returns a shooting interval for this enemy as a bullet
     */
    static double interval();
};


template<>
class Constants<SuperShootingEnemy>
{
public:
    static sad::Sprite2D::Options * sprite();
    static sad::p2d::CollisionShape * shape();
};

template<
    typename _ObjectType
>
void initFromConstants(_ObjectType* me)
{
    sad::Sprite2D::Options* options = Constants<_ObjectType>::sprite();
    me->setOptions(options);
    delete options;
    me->setShape(Constants<_ObjectType>::shape());
}
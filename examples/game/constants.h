/*! \file constants.h
	\author HiddenSeeker

	Describes game object  constants, used to initialize sprites for
	various game objects.

	Game object constants contain various rendering parameters, such as
	name of texture, texture coordinates and size of polygon, which is
	texture and texture coordinates mapped to.

	Also they contain shape templates.
 */
#pragma once
#include <p2d/app/constants.h>


class Player;
class PlayerBullet;
class EnemyBullet;
class Bonus;
class Enemy;
class ShootingEnemy;
class SuperShootingEnemy;

namespace sad
{

namespace p2d
{

namespace app
{

template<>
class Constants<Player>
{
public:
	static Sprite2DAdapter::Options * sprite();
	static sad::p2d::CollisionShape * shape();
};



template<>
class Constants<PlayerBullet>
{
public:
	static Sprite2DAdapter::Options * sprite();
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
	static Sprite2DAdapter::Options * sprite();
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
	static Sprite2DAdapter::Options * sprite();
	static sad::p2d::CollisionShape * shape();
};


template<>
class Constants<Enemy>
{
public:
	static Sprite2DAdapter::Options * sprite();
	static sad::p2d::CollisionShape * shape();
};


template<>
class Constants<ShootingEnemy>
{
public:
	static Sprite2DAdapter::Options * sprite();
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
	static Sprite2DAdapter::Options * sprite();
	static sad::p2d::CollisionShape * shape();
};


}

}

}

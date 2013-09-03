/*! \file WorldObjectConstants.h
	\author HiddenSeeker

	Describes game object  constants, used to initialize sprites for
	various game objects.

	Game object constants contain various rendering parameters, such as
	name of texture, texture coordinates and size of polygon, which is
	texture and texture coordinates mapped to.

	Also they contain shape templates.
 */
#include <sprite2dadapter.h>
#include <p2d/circle.h>
#include <p2d/line.h>
#include <p2d/rectangle.h>
#pragma once


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
class WorldObjectConstants
{
public:
	/*! Returns a sprite options, needed to create sprite
		\return sprite options
	 */
	static Sprite2DAdapter::Options * sprite();
	/*! Returns shape needed for physical engine
	 */
	static p2d::CollisionShape * shape();
};

class Player;
class PlayerBullet;
class EnemyBullet;
class Bonus;
class Enemy;
class ShootingEnemy;
class SuperShootingEnemy;

template<>
class WorldObjectConstants<Player>
{
public:
	static Sprite2DAdapter::Options * sprite();
	static p2d::CollisionShape * shape();
};

template<>
class WorldObjectConstants<PlayerBullet>
{
public:
	static Sprite2DAdapter::Options * sprite();
	static p2d::CollisionShape * shape();
	/*! Describes a velocity for bullet
		\return velocity
	 */
	static double velocity();
	/*! Returns a shooting interval with this bullet type
	 */
	static double interval();
};


template<>
class WorldObjectConstants<EnemyBullet>
{
public:
	static Sprite2DAdapter::Options * sprite();
	static p2d::CollisionShape * shape();
	/*! Describes a velocity for bullet
		\return velocity
	 */
	static double velocity();
	/*! Returns a shooting interval with this bullet type
	 */
	static double interval();
};


template<>
class WorldObjectConstants<Bonus>
{
public:
	static Sprite2DAdapter::Options * sprite();
	static p2d::CollisionShape * shape();
};


template<>
class WorldObjectConstants<Enemy>
{
public:
	static Sprite2DAdapter::Options * sprite();
	static p2d::CollisionShape * shape();
};


template<>
class WorldObjectConstants<ShootingEnemy>
{
public:
	static Sprite2DAdapter::Options * sprite();
	static p2d::CollisionShape * shape();
	/*! Describes a velocity for shooting enemy as a bullet
		\return velocity
	 */
	static double velocity();
	/*! Returns a shooting interval for this enemy as a bullet
	 */
	static double interval();
};


template<>
class WorldObjectConstants<SuperShootingEnemy>
{
public:
	static Sprite2DAdapter::Options * sprite();
	static p2d::CollisionShape * shape();
};

/*! \file WorldObjectConstants.h
	\author HiddenSeeker

	Describes game object  constants, used to initialize sprites for
	various game objects.

	Game object constants contain various rendering parameters, such as
	name of texture, texture coordinates and size of polygon, which is
	texture and texture coordinates mapped to.

	Also they contain shape templates.
 */
#include <p2d/app/constants.h>
#pragma once



class Player;
class PlayerBullet;
class EnemyBullet;
class Bonus;
class Enemy;
class ShootingEnemy;
class SuperShootingEnemy;
class GridNode;
class GridNodeEdge;

template<>
class p2d::app::Constants<Player>
{
public:
	static Sprite2DAdapter::Options * sprite();
	static p2d::CollisionShape * shape();
};

template<>
class p2d::app::Constants<PlayerBullet>
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
class p2d::app::Constants<EnemyBullet>
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
class p2d::app::Constants<Bonus>
{
public:
	static Sprite2DAdapter::Options * sprite();
	static p2d::CollisionShape * shape();
};


template<>
class p2d::app::Constants<Enemy>
{
public:
	static Sprite2DAdapter::Options * sprite();
	static p2d::CollisionShape * shape();
};


template<>
class p2d::app::Constants<ShootingEnemy>
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
class p2d::app::Constants<SuperShootingEnemy>
{
public:
	static Sprite2DAdapter::Options * sprite();
	static p2d::CollisionShape * shape();
};


template<>
class p2d::app::Constants<GridNode>
{
public:
	static Sprite2DAdapter::Options * sprite();
	static p2d::CollisionShape * shape();
};

template<>
class p2d::app::Constants<GridNodeEdge>
{
public:
	static Sprite2DAdapter::Options * sprite();
};
/*! \file constants.h
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


class GridNode;
class GridNodeEdge;
class Ball;
class UncoloredBullet;
class Platform;
class Shooter;
class ColoredBullet;
class MovingSmile;


namespace sad
{

namespace p2d
{

namespace app
{


template<>
class Constants<ColoredBullet>
{
public:
	static Sprite2DAdapter::Options * sprite();
	static p2d::CollisionShape * shape();
};

template<>
class Constants<Ball>
{
public:
	static Sprite2DAdapter::Options * sprite();
	static p2d::CollisionShape * shape();
};


template<>
class Constants<GridNode>
{
public:
	static Sprite2DAdapter::Options * sprite();
	static p2d::CollisionShape * shape();
};

template<>
class Constants<GridNodeEdge>
{
public:
	static Sprite2DAdapter::Options * sprite();
	static hRectF sourceRect();
};

template<>
class Constants<UncoloredBullet>
{
public:
	static Sprite2DAdapter::Options * sprite();
	static p2d::CollisionShape * shape();
};


template<>
class Constants<Platform>
{
public:
	static Sprite2DAdapter::Options * sprite();
	static p2d::CollisionShape * shape();
};

template<>
class Constants<Shooter>
{
public:
	static Sprite2DAdapter::Options * sprite();
	static p2d::CollisionShape * shape();
};


template<>
class Constants<MovingSmile>
{
public:
	static Sprite2DAdapter::Options * sprite();
	static p2d::CollisionShape * shape();
};

}

}

}

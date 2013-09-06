/*! \file constants.h
	\author HiddenSeeker

	Describes  object  constants, used to initialize sprites for
	various objects.

	Object constants contain various rendering parameters, such as
	name of texture, texture coordinates and size of polygon, which is
	texture and texture coordinates mapped to.

	Also they contain collision shape templates.
 */
#include "../../sprite2dadapter.h"
#include "../circle.h"
#include "../line.h"
#include "../rectangle.h"
#pragma once


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
	static Sprite2DAdapter::Options * sprite();
	/*! Returns shape needed for physical engine
	 */
	static p2d::CollisionShape * shape();
};

}

}

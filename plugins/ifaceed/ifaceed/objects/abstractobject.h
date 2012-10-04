/** \file abstractobject.h
	\author HiddenSeeker

	Describes an abstract screen template object
 */
#include "scene.h"
#include "marshal/serializableobject.h"
#include "templates/refcountable.h"
#pragma once

class ScreenTemplate;

/** Describes an abstract object of screen template
 */
class AbstractObject: public sad::BasicNode, public SerializableObject, public RefCountable
{
 SAD_NODE

};

